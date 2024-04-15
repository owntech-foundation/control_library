# Controller 

`Controller` is an abstract class.

The main idea is to fix the interface of a `controller`.

Each type of controller like `Pid`, `RST` and `Pr` have then the same way of life.

It has been thinking considering SISO (Single Input Single Output) and sample controller.

If you want to implement a new controller type, you should consider to inherit from this
class.

