function v = helics_property_int_console_log_level()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 84);
  end
  v = vInitialized;
end
