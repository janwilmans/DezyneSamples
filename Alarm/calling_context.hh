#ifndef CALLING_CONTEXT_HH
#define CALLING_CONTEXT_HH

#include <iostream>
#include <vector>
#include <memory>

struct calling_context
{
  mutable std::shared_ptr<std::vector<std::exception_ptr>> exceptions;

  calling_context()
      : exceptions(std::make_shared<std::vector<std::exception_ptr>>())
  {
  }
 
  calling_context& operator= (const calling_context&) = delete;

  void operator()()
  {
      if(exceptions->size() == 1u)
      {
          std::rethrow_exception((*exceptions)[0]);
      }
      else if(exceptions->size() > 1u)
      {
          throw std::exception("Multiple exceptions");
      }
  }
  void extend(std::exception_ptr&& e) const
  {
      std::string what;
      try
      {
          std::rethrow_exception(e);
      }
      catch(const std::exception& r)
      {
          what = r.what();
      }
      catch(...)
      {
          what = "Unknown exception";
      }

      std::cout << "record exception #" << exceptions->size() << ": " << what << std::endl;
      exceptions->emplace_back(std::move(e));
  }
};

#endif
