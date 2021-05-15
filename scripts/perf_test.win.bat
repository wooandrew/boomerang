@echo off
echo            ,,...--------..      
echo       -/oshdmNMMMMMMMMMMMMmho. 
echo   ~`hHmm````````````-mMMMMMMMN.
echo                       `:hMMMMMs
echo                          /MMMMy
echo     Project Boomerang     hMMMo
echo  A.W. P.S. B.Y. A.K.      yMMM:
echo                           dMMN 
echo                          -MMMo 
echo                          yMMN` 
echo                         -MMM+   
echo                         dMMd    Boomerang 2wv0.1.0-pre.3-alpha
echo                        -MMN.    Performance Tester for Windows   
echo                        oMN-         by Andrew Woo
echo                        `/`  
echo.
echo  Gathering dxdiag data...
dxdiag /t dxdiag.out
echo  DONE.
echo.
echo  Starting Boomerang in benchmark mode...
boomerang.exe -d TRUE -m BENCH -o bench.out > runtime 2>&1
echo  DONE.
echo.
echo  Converting data to b64==...
echo      -> dxdiag.out...
:: TODO
echo         DONE.
echo      -> bench.out...
:: TODO
echo         DONE.
echo      -> runtime...
:: TODO
echo         DONE.
echo.
echo  Sending diagnostics data...
:: TODO
echo  DONE.
echo.
echo  Performance testing complete. You may close this window and
echo   remove any remaining files/folders.
echo.
pause
