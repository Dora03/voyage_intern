clear all; close all;

[y, fsy] = audioread('tsp.wav');
afinfok = audioinfo('tsp.wav');

numfiles = 9;
mydata = cell(1, numfiles);
for abc = 1:numfiles
  fname = sprintf('tsp_m%d.wav', abc);

[x,fsx]=audioread(fname);

fx = fft(x, length(y));
fy = fft(y);
plot(fy)
z = fx ./ fy;
xz = real(ifft(z));

%% plot
xz = 0.9*xz/(max(abs(xz)));
plot([xz(end-(end+1)/2:end); xz(1:end-(end+1)/2-1)]);
fnameout = sprintf('ir_%d.wav',abc);
audiowrite(fnameout, [xz(end-(end+1)/2:end); xz(1:end-(end+1)/2-1)], fsx, 'BitsPerSample', 16);

end