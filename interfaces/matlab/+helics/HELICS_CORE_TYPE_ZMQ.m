function v = HELICS_CORE_TYPE_ZMQ()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1946183068);
  end
  v = vInitialized;
end
