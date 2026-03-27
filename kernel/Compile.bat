cd /d E:\Project\webcad\kernel\build\wasm
del main.js
del main.wasm
emmake make -j24

cd /d C:\Project\webcad\Project\kernel_app\wasm
del main.js
del main.wasm
emmake make -j3
 
# macos
cd /Users/Project/webcad/Project/kernel_app/wasm
emmake make -j7
cd /Users/Project/webcad/Project/kernel_app/wasm
emmake make -j9
cd /Users/Project/webcad/Project/kernel_app/wasm
emmake make -j24

open /Applications/Google\ Chrome.app --args --enable-features=SharedArrayBuffer


# liunx
cd /home/liao/SDK/emsdk
source ./emsdk_env.sh
cd /home/liao/Project/webcad/Project/kernel_app/wasm
emmake make -j

google-chrome --args --enable-features=SharedArrayBuffer



cd /d C:\Project\webcad\Project\kernel_app\build
del main.js
del main.wasm
emmake make -j