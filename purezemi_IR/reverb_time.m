function rt = reverb_time(x, fs, Tnum, T)
%REVERB_TIME   Compute Reverberation Time (RT60)
%   rt = reverb_time(x, fs);
%   Compute RT60 from T30 (least square fit on reverberation envelope
%   at -5dB and -35dB).
%
%   rt = reverb_time(x, fs, true); draws figure of reverb time
%
%   rt = reverb_time(x, fs, true, [t1 t2 t3]); draws figure of reverb
%   time with least square fit on reverb envelope from t1 dB to t2 dB
%   and extrapolate the line to t3 dB (by default, t1=-5, t2=-35, t3=-60).
%
%   2008-11-17 MARUI Atsushi

if nargin>3
  T1 = T(1);
  T2 = T(2);
  T3 = T(3);
else
  T1 = -5;
  T2 = -25; %T20
  T3 = -60;
end

re = reverb_envelope(x);

redb = pow2db(abs(re));
mm = max(redb);
t = ([0:length(re)-1]/fs)';
z = redb-mm;
ind = [sum(z >= T1)  sum(z >= T2)];

% linear fit (-1dB ~ -21dB) and find the time of being -60dB
p = polyfit(z(ind(1):ind(2)), t(ind(1):ind(2)), 1);
rt = polyval(p, T3) - t(ind(1));

% c = 60/Tnum;

% rt = (ind(2)-ind(1))*c/fs; %easily