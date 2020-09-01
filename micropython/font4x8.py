import framebuf

chr1 = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00000000
  ]), 8, 8, framebuf.MONO_HLSB)

chr2 = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b00010000,
  0b01110000,
  0b01000000,
  0b01000000,
  0b01110000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chr3 = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b00010000,
  0b00110000,
  0b00010000,
  0b00010000,
  0b01110000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chr4 = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01010000,
  0b01010000,
  0b01110000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chr5 = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b01000000,
  0b01110000,
  0b00010000,
  0b00010000,
  0b01110000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chr6 = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b01000000,
  0b01110000,
  0b01010000,
  0b01010000,
  0b01110000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chr7 = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b00010000,
  0b00100000,
  0b00100000,
  0b00100000,
  0b00100000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chr8 = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b01010000,
  0b01110000,
  0b01010000,
  0b01010000,
  0b01110000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chr9 = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b01010000,
  0b01110000,
  0b00010000,
  0b00010000,
  0b01110000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chr0 = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b01010000,
  0b01010000,
  0b01010000,
  0b01010000,
  0b01110000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chrA = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b01010000,
  0b01110000,
  0b01010000,
  0b01010000,
  0b01010000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chrB = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01100000,
  0b01010000,
  0b01100000,
  0b01010000,
  0b01010000,
  0b01100000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chrC = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b00100000,
  0b01010000,
  0b01000000,
  0b01000000,
  0b01010000,
  0b00100000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chrD = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01100000,
  0b01010000,
  0b01010000,
  0b01010000,
  0b01010000,
  0b01100000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chrE = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b01000000,
  0b01100000,
  0b01000000,
  0b01000000,
  0b01110000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

chrF = framebuf.FrameBuffer(bytearray([
  0b00000000,
  0b01110000,
  0b01000000,
  0b01100000,
  0b01000000,
  0b01000000,
  0b01000000,
  0b00000000 
  ]), 8, 8, framebuf.MONO_HLSB)

font = { "1":chr1, "2":chr2, "3":chr3, "4":chr4, "5":chr5, "6":chr6, "7":chr7, "8":chr8, "9":chr9, "0":chr0, "A":chrA, "B":chrB, "C":chrC, "D":chrD, "E":chrE, "F":chrF,
           1:chr1, 2:chr2, 3:chr3, 4:chr4, 5:chr5, 6:chr6, 7:chr7, 8:chr8, 9:chr9, 0:chr0, 10:chrA, 11:chrB, 12:chrC, 13:chrD, 14:chrE, 15:chrF }
