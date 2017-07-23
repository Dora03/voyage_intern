clear all; close all;

numfiles =  9;
mydata = cell(1,numfiles);

[y,fsy] = audioread('tsp.wav');
for abc = 1:numfiles
    fname = sprintf('tsp_%d.wav',abc);
[x,fsx] = audioread(fname);

t01 = finddelay(y,x(1:length(y)))

x1 = x(t01:t01+length(y));

xz = x1;
xz = 0.9*xz/max(abs(xz));
fnameout = sprintf('tsp_m%d.wav',abc);
fname_out = sprintf(fnameout);
audiowrite(fname_out,xz,fsy,'BitsPerSample',16);

fig = figure(1);
plot(xz);
pngname = sprintf('plot_tsp_%d.png',abc);
saveas(fig,pngname,'png')
end