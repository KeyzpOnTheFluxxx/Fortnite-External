
#define M_PI 3.14159265358979323846f
#define UCONST_Pi           3.1415926

class Vector
{
public:
	Vector() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector()
	{

	}

	float x;
	float y;
	float z;

	inline float Dot(Vector v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	Vector operator+(Vector v)
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector operator-(Vector v)
	{
		return Vector(x - v.x, y - v.y, z - v.z);
	}


};