function v = helics_query_mode_fast()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 115);
  end
  v = vInitialized;
end
