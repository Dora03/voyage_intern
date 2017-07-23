clear all; close all;
%% rt
numfiles = 9;
mydata = cell(1, numfiles);
rt_m = 0;
% a_m = 0;
result = zeros(numfiles,3);
for abc = 1:numfiles
  filename = sprintf('ir_%d.wav', abc);

[x,fs]=audioread(filename);

T = [-1 -21 -60];
rt = reverb_time(x,fs,20,T);
Tnum = 20;
if rt > 10
    Tnum = 15;
    T = [-1 -1-Tnum -60];
    rt = reverb_time(x,fs,Tnum,T);
end
if rt > 10
    Tnum = 10;
    T = [-1 -1-Tnum -60];
    rt = reverb_time(x,fs,Tnum,T);
end
if rt > 10
    Tnum = 5;
    T = [-1 -1-Tnum -60];
    rt = reverb_time(x,fs,Tnum,T);
end

result(abc,1) = abc;
result(abc,2) = rt;
result(abc,3) = Tnum;
rt_m = rt_m + rt;
end
rt_m = rt_m/numfiles;
save('rt_result.txt','result','rt_m','-ascii');