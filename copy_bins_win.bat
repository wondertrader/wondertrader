@echo off
set despath=%1
if "%despath%"=="" set despath="../wtpy"
python ./scripts/copy_bins_win.py %despath%
