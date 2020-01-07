{
  a = "test file";
  list = [ "a" "b" "c" ];
  objs = {
    first = "first";
    second = "second";
  };
  namevalues = [
    { name = "first"; value = 1; }
    { name = "second"; value = 2; }
  ];
  files = {
    currentDir = ./.;
  };
}
