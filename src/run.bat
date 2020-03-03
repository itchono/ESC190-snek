@echo OFF
FOR /L %%A IN (1,1, 20000) DO (
  python main.py
  timeout /t 1
)