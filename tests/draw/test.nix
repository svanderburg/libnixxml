{
  dimensions = {
    width = 200;
    height = 200;
  };

  figures = {
    shape1 = {
      type = "rectangle";
      width = 20;
      height = 20;
    };

    shape2 = {
      type = "rectangle";
      width = 20;
      height = 30;
    };

    shape3 = {
      type = "circle";
      radius = 20;
    };
  };

  draw = [
    { figure = "shape1"; x = 10; y = 10; }
    { figure = "shape1"; x = 20; y = 10; }
    { figure = "shape2"; x = 10; y = 20; }
    { figure = "shape3"; x = 50; y = 50; }
  ];

  meta = {
    author = "Sander van der Burg";
    description = "Silly test case";
  };

  tags = [
    "silly"
    "example"
  ];
}
