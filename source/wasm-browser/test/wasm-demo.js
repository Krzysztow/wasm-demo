const { expect, assert } = require('chai');
const Module = require('../wasmDemo');
const { doCompress, doUncompress } = require('../compress');

before(() => {
  return new Promise((resolve) => {
    Module.onRuntimeInitialized = resolve;
  });
});


const DATA_STRING = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
const DATA = new TextEncoder("utf-8").encode(DATA_STRING);

const compress = (dataIn, outputBufferSize=0) => {
  return doCompress(dataIn, outputBufferSize);
};


const decompress = (data, outputBufferSize) => {
  return doUncompress(data, outputBufferSize);
};



describe('Compression', () => {
  describe('sanity', () => {


    it('should fail when passed wrong parameters', () => {
      expect(Module._demo_compress(0, 0, 0, 0)).to.equal(0);
    });


    it('should fail when output buffer too small', () => {
      const nonCompressibleData = new Uint8Array(128).map((_, i) => i);

      assert.throws(
        () => compress(nonCompressibleData, nonCompressibleData.length/2),
        "Couldn't perform compression/uncompression");
    });


    it('should compress when sensible parameters are passed', () => {
      const compressedData = compress(DATA);

      expect(compressedData.length).to.not.equal(0);
      expect(compressedData.length).to.be.lessThan(DATA.length);
    });


    it('should compress to larger buffer when non-compressible data is passed', () => {
      const nonCompressibleData = new Uint8Array(128).map((_, i) => i);

      const compressedData = compress(nonCompressibleData);

      expect(compressedData.length).to.not.equal(0);
      expect(compressedData.length).to.be.greaterThan(nonCompressibleData.length);
    });


    it('should compress and de-compress to the same data', () => {
      const compressedData = compress(DATA);
      const decompressedData = decompress(compressedData, DATA.length);

      expect(decompressedData.length).to.equal(DATA.length);
      expect(decompressedData).to.be.eql(DATA);
    });


  });
});
