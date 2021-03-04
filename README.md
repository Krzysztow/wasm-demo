# Demo application for WASM and native C++/CMake build

## Accompanying presentation:
This application was written for the Rust & C++ Meetup event happening on 1st of March 2021. There's an associated presentation at [slides.com](https://slides.com/krzysztofwie/wading-into-wasm).

## The goals:
The goal of this app was to write a native application and then evolve it into WASM-enabled one, with sharing as much code as possible.
The native application is performing file compression (and in some limited cases decompression). On the browser, we're interested only on compression.

Note: 
Have a look of the git history of the `master` branch; it tells the story!

Disclaimer:
This application may be buggy (that was just a demo) and has minimal set of tests. 

## Limitations:
Currently, the application has limitations like:
* Decryption on the native cmd line tool doesn't work for all the inputs - it's because we handle just a simple case (decrypt all at once) and we have to guess the output buffer size (we assume 2x input buffer, which might not be correct, when the compression ratio is high)
* Encryption on the browser has limits to the file sizes - the WASM runtime has limited heap associated, and for the encryption to happen, we need to alloc buffers for both input and output. For bigger files it exceeds the heap size limit (can be increased during compilation; or we should perform chunking).

## Build 
* When building for native, use `-DFOR_NATIVE=On` cmake configuration flag. The two binaries produced (gtests + compression-tool) are executable on the command line;
* When building for WASM, the output files `wasmDemo` and `wasmDemo.js` need to be bundled with `sources/wasm-browser/{index.html, index.css}` and served by the server (e.g. `python3 -m http.server`). We depend on the [Emscripten](https://github.com/emscripten-core/emscripten) toolchain, and the command to compile:
```
emcmake cmake -DFOR_WASM=On <source-root>` 
cmake --build .
```

Note: There's a (not-yet-working) `feature/CI` branch with the build steps documenting this process.
