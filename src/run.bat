FOR /L %%A IN (1,1, 1000) DO (
  ECHO %%A
  python main.py
  timeout /t 1
)