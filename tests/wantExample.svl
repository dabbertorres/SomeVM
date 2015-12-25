// comment

/* block
 * comment
 */

var x = 5;	// type inference variable declaration

var: string s = "Hello!";	// type specified variable declaration

// if statement
if(x == 5)
{
	x = 10;
	io::println(s);
};	// note the ending semicolon

// function declaration with type inference for return type
// must specify argument types, but can give multiple types
// function type is inferred
var f = (var: int | float y)
{
	return y * 2;
};

// function declaration with return type specified
var g = (var: string str): string
{
	return str += " sent to function g";
};

// function type is specified
// note that variable names are not required until actual function value
var: (var: int | float, var: int | float): int | float mult = (var: int | float x, var: int | float y): int | float
{
	return x * y;
};

// long and ugly huh?
// try this:
typedef multFunc = (var: int | float, var: int | float): int | float;

var mf: multFunc = (var: int | float x, var: int | float y): int | float
{
	return x * y;
};

// not as bad. Though, I'd save that for when you need multiple variables of the same type

// function call:
var ret = f(5.2);	// ret = 10.4

// constants: assigned at run time
const var l = 42;

// constant expressions: assigned at compile time
const someStr = "String for something";

// classes:
// constructor is declared in place
// like functions, multi type arguments can be done for classes as well
class Car(var h: int)
{
	private cvar horsePower = h;
							
	// svar = "static var". static vars persist through construction/destruction
	private svar count++;	// variables are initialized to 0, null, etc if not specified.
							// In this case, count will be 1 on the first Car created. 
							// And then increment for every Car created.
	
	public var getHorsePower = ()	// notice this is a "var", not a "cvar". Yes, class functions can be changed at runtime
	{
		return horsePower;
	};
	
	// override destructor
	destroy
	{
		count--;
	};
};

// inheritance
class Honda(var h: int, var ts: int) = Car(h)
{
	// property
	public var topSpeed = ts
	{
		get()
		{
			return this;
		};
		
		set(var v)
		{
			topSpeed = v;
		};
	};
};
