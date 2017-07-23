function y = reverb_envelope(x)
% REVERB_ENVELOPE   Calculates reverberation envelope
%    y=reverb_envelope(x) calculates reverberation envelope y from
%    an impulse response x.  This function can be used to see how
%    the reverberation decays in the corresponding room.
%
%    Reference: Ooga, Yamazaki, and Kaneda
%              "Acoustic Systems and the Digital Processing" (p.163)
%
% 2007-05-28 MARUI Atsushi

%if error
if ndims(x)>2
  return;
end

%if error
if size(x,1) < size(x,2)
  x = x';
end

y = flipud(cumsum(flipud(x .^ 2))); %.^ : ”z—ñ‚Ì‚×‚«æ