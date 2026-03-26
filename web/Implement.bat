del .\dist\main.js
del .\dist\main.wasm
del .\dist\main.worker.js
copy ..\Kernel\bin\wasm\main.js .\dist\main.js
copy ..\Kernel\bin\wasm\main.worker.js .\dist\main.worker.js
copy ..\Kernel\bin\wasm\main.wasm .\dist\main.wasm

