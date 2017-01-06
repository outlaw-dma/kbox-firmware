/*
  The MIT License

  Copyright (c) 2017 Thomas Sarlandie thomas@sarlandie.net

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <string.h>

enum KommandIdentifier {
  KommandPing = 0x00,
  KommandPong = 0x01,
  KommandErr = 0x0F,
  KommandLog = 0x10,
  KommandScreenshot = 0x30,
  KommandScreenshotData = 0x31,
};

template <uint16_t maxSize> class Kommand {
  uint8_t _bytes[maxSize];
  size_t _index;

  public:
    Kommand(KommandIdentifier id) : _index(0) {
      append16(id);
    };

    void append16(uint16_t w) {
      if (_index + 2 > maxSize) {
        return;
      }
      _bytes[_index++] = w & 0xff;
      _bytes[_index++] = (w>>8) & 0xff;
    }

    void append8(uint8_t b) {
      if (_index + 1 > maxSize) {
        return;
      }
      _bytes[_index++] = b;
    };

    void appendNullTerminatedString(const char *s) {
      int len = strlen(s);

      // We want to have space for the string content + the terminating \0
      if (len > maxSize - _index - 1) {
        len = maxSize - _index - 1;
      }
      if (len <= 0) {
        return;
      }

      strncpy((char*)_bytes + _index, s, len);
      _index += len;

      // Always add 0 because strncpy(a,b, strlen(s)) will not!
      append8((uint8_t) 0);
    }

    void captureBuffer(uint8_t **ptr, size_t **index) {
      *ptr = _bytes;
      *index = &_index;
    }
};

