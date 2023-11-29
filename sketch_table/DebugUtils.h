#if DEBUG
  #define DEBUG_PRINT(x) \
  Serial.print(millis()); \
  Serial.print(' : '); \
  Serial.print(__FILE__); \
  Serial.print(' : '); \
  Serial.print(__LINE__); \
  Serial.print(' : '); \
  Serial.println(x);
#else
  #define DEBUG_PRINT(x)
#endif