@echo OFF
FOR /L %%A IN (1,1, 20) DO (
  ECHO %%A
  python main.py
  timeout /t 1
)