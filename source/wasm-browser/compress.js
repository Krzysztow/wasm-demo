(function(exports){

  const IS_NODE = (typeof process === 'object' && typeof require === 'function');
  
  if (IS_NODE) {
    var Module = require('./wasmDemo');
  }
  else {
    var Module = window.Module;
  }


  function performComOrUncompress(fSize, sSize, comOrUnCompressFct) {
    try {
      const fBuffer = alloc(fSize);
      if (0 === fBuffer) {
        throw new Error("Couldn't allocate data for the input buffer");
      }
      var fBufferOffset = fBuffer.offset;
  
      const sBuffer = alloc(sSize);
      if (0 === fBuffer) {
        throw new Error("Couldn't allocate data for the output buffer");
      }
      var sBufferOffset = sBuffer.offset;
  
      const compressResult = comOrUnCompressFct(fBuffer, sBuffer);
      if (0 === compressResult) {
        throw new Error("Couldn't perform compression/uncompression");
      }
  
      const retBuffer = new Uint8Array(compressResult);
      retBuffer.set(sBuffer.buffer.slice(0, compressResult));
      return retBuffer;
    }
    finally {
      freeCond(fBufferOffset);
      freeCond(sBufferOffset);
    }
  };

  function alloc(size) {
    var bufferOffset = Module._malloc(size);
    const buffer = Module.HEAP8.subarray(bufferOffset, bufferOffset + size);
    return {
        buffer: buffer,
        offset: bufferOffset,
    };
  }

  function freeCond(buffOffset) {
    if (buffOffset) Module._free(buffOffset);
  };

  exports.doCompress = function(data, outputBufferSize) {
    outputBufferSize = outputBufferSize || Module._demo_maxCompressSize(data.length);

    const compressedData = performComOrUncompress(data.length, outputBufferSize, (inBuffer, outBuffer) => {
        inBuffer.buffer.set(data);
        return Module._demo_compress(inBuffer.offset, inBuffer.buffer.length,
                                                        outBuffer.offset, outBuffer.buffer.length);
    });

    return compressedData;
  };

  exports.doUncompress = function(data, outputBufferSize) {
    const compressedData = performComOrUncompress(data.length, outputBufferSize, (inBuffer, outBuffer) => {
        inBuffer.buffer.set(data);
        return Module._demo_decompress(inBuffer.offset, inBuffer.buffer.length,
                                    outBuffer.offset, outBuffer.buffer.length);
    });

    return compressedData;
  };

})(typeof exports === 'undefined'? this['Compress']={}: exports);
