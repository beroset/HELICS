function v = HELICS_PROPERTY_TIME_RT_LEAD()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 88);
  end
  v = vInitialized;
end
