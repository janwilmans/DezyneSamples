# run tests
Tests\Release\Tests.exe -s -i -w NoTests -r junit -o junit-results.xml

# upload results to AppVeyor
$wc = New-Object 'System.Net.WebClient'
$wc.UploadFile("https://ci.appveyor.com/api/testresults/junit/$($env:APPVEYOR_JOB_ID)", (Resolve-Path .\junit-results.xml))