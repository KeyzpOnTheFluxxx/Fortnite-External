
enum Bones
{
	BONE_NULL_1 = 0,
	BONE_NULL_2 = 1,
	BONE_PELVIS_1 = 2,
	BONE_PELVIS_2 = 3,
	BONE_PELVIS_3 = 4,
	BONE_TORSO = 5,

	BONE_CHEST_LOW = 6,
	BONE_CHEST = 7,

	// -------------------------

	BONE_CHEST_LEFT = 8,

	BONE_L_SHOULDER_1 = 9,
	BONE_L_ELBOW = 10,

	BONE_L_HAND_ROOT_1 = 11,

	BONE_L_FINGER_1_ROOT = 12,
	BONE_L_FINGER_1_LOW = 13,
	BONE_L_FINGER_1 = 14,
	BONE_L_FINGER_1_TOP = 15,

	BONE_L_FINGER_2_ROOT = 16,
	BONE_L_FINGER_2_LOW = 17,
	BONE_L_FINGER_2 = 18,
	BONE_L_FINGER_2_TOP = 19,

	BONE_L_FINGER_3_ROOT = 20,
	BONE_L_FINGER_3_LOW = 21,
	BONE_L_FINGER_3 = 22,
	BONE_L_FINGER_3_TOP = 23,

	BONE_L_FINGER_4_ROOT = 24,
	BONE_L_FINGER_4_LOW = 25,
	BONE_L_FINGER_4_ = 26,
	BONE_L_FINGER_4_TOP = 27,

	BONE_L_THUMB_ROOT = 28,
	BONE_L_THUMB_LOW = 29,
	BONE_L_THUMB = 30,

	BONE_L_HAND_ROOT_2 = 31,
	BONE_L_WRIST = 32,
	BONE_L_ARM_LOWER = 33,

	BONE_L_SHOULDER_2 = 34,

	BONE_L_ARM_TOP = 35,

	// -------------------------

	BONE_CHEST_TOP_1 = 36,

	// -------------------------

	BONE_CHEST_RIGHT = 37,

	BONE_R_ELBOW = 38,

	BONE_R_HAND_ROOT_1 = 39,

	BONE_R_FINGER_1_ROOT = 40,
	BONE_R_FINGER_1_LOW = 41,
	BONE_R_FINGER_1 = 42,
	BONE_R_FINGER_1_TOP = 43,

	BONE_R_FINGER_2_ROOT = 44,
	BONE_R_FINGER_2_LOW = 45,
	BONE_R_FINGER_2 = 46,
	BONE_R_FINGER_2_TOP = 47,

	BONE_R_FINGER_3_ROOT = 48,
	BONE_R_FINGER_3_LOW = 49,
	BONE_R_FINGER_3 = 50,
	BONE_R_FINGER_3_TOP = 51,

	BONE_R_FINGER_4_ROOT = 52,
	BONE_R_FINGER_4_LOW = 53,
	BONE_R_FINGER_4_ = 54,
	BONE_R_FINGER_4_TOP = 55,

	BONE_R_THUMB_ROOT = 56,
	BONE_R_THUMB_LOW = 57,
	BONE_R_THUMB = 58,

	BONE_R_HAND_ROOT = 59,
	BONE_R_WRIST = 60,
	BONE_R_ARM_LOWER = 61,

	BONE_R_SHOULDER = 62,

	BONE_R_ARM_TOP = 63,

	// -------------------------

	BONE_CHEST_TOP_2 = 64,

	BONE_NECK = 65,
	BONE_HEAD = 66,

	// -------------------------

	BONE_L_LEG_ROOT = 67,
	BONE_L_KNEE = 68,
	BONE_L_FOOT_ROOT = 69,
	BONE_L_SHIN = 70,
	BONE_L_FOOT_MID = 71,
	BONE_L_FOOT_LOW = 72,
	BONE_L_THIGH = 73,

	// -------------------------

	BONE_R_LEG_ROOT = 74,
	BONE_R_KNEE = 75,
	BONE_R_FOOT_ROOT = 76,
	BONE_R_SHIN = 77,
	BONE_R_FOOT_MID = 78,
	BONE_R_FOOT_LOW = 79,
	BONE_R_THIGH = 80,

	// -------------------------

	BONE_NULL_3 = 81,
	BONE_MISC_L_FOOT = 82,
	BONE_MISC_R_FOOT = 83,
	BONE_NULL_4 = 84,
	BONE_MISC_R_HAND_1 = 85,
	BONE_MISC_L_HAND = 86,
	BONE_MISC_R_HAND_2 = 87,
};
struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};
struct FTransform
{
	FQuat rot;
	Vector translation;
	char pad[4];
	Vector scale;
	char pad1[4];
	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

enum E_COMMAND_CODE
{
	ID_NULL = 0,	//
	ID_READ_PROCESS_MEMORY = 5,	// 
	ID_READ_KERNEL_MEMORY = 6,	// 
	ID_WRITE_PROCESS_MEMORY = 7,	//
	ID_GET_PROCESS = 10,	//
	ID_GET_PROCESS_BASE = 11,	//
	ID_GET_PROCESS_MODULE = 12,	//
	ID_GET_WND_THREAD = 15,	//
	ID_SET_WND_THREAD = 16,	//
	ID_GET_DRIVER_BASE = 20,	//
	ID_REMOVE_CACHE_IMAGE = 25	//
};
typedef struct _MEMORY_STRUCT
{
	uint64_t	process_id;
	void*		address;
	uint64_t	size;
	uint64_t	size_copied;
	void*		buffer;
	uint64_t	struct_value;
} MEMORY_STRUCT, *PMEMORY_STRUCT;
typedef struct _MEMORY_STRUCT2
{
	BYTE type;
	LONG usermode_pid;
	LONG target_pid;
	ULONG64 base_address;
	void* address;
	LONG size;
	void* output;
}MEMORY_STRUCT2, *PMEMORY_STRUCT2;

template<typename ... A>
uint64_t call_driver_control(void* control_function, const A ... arguments)
{
	if (!control_function)
		return 0;

	using tFunction = uint64_t(__stdcall*)(A...);
	const auto control = static_cast<tFunction>(control_function);

	return control(arguments ...);
}
void* kernel_control_function()
{
	HMODULE hModule = LoadLibrary(xorstr_(L"win32u.dll"));

	//printf(xorstr_("> hModule: %llx\n"), hModule);

	if (!hModule)
		return nullptr;

	void* func = reinterpret_cast<void*>(GetProcAddress(hModule, xorstr_("NtSetCompositionSurfaceStatistics")));

	//printf(xorstr_("> func: %llx\n"), func);

	return func;
}
uint64_t read_kernel(void* control_function, uint64_t address, void* buffer, std::size_t size)
{
	return call_driver_control(control_function, ID_READ_KERNEL_MEMORY, address, buffer, size);
}

void* m_driver_control;
DWORD64 m_pid;
DWORD64 usermode_pid;
DWORD64 m_base;
DWORD64 m_game_manager;
DWORD64 isin_game;

DWORD64 GetBaseAddress()
{
	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 12;
	memory_struct.usermode_pid = m_pid;

	int result = call_driver_control(m_driver_control, &memory_struct);

	return result == 0 ? memory_struct.base_address : 0;
}

int ClearPiddb()
{
	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 2;
	int result = call_driver_control(m_driver_control, &memory_struct);
	return result;
}
int FindMmDriverData()
{
	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 8;
	int result = call_driver_control(m_driver_control, &memory_struct);
	return result;
}
int ClearMmUnloadedDrivers()
{
	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 9;
	int result = call_driver_control(m_driver_control, &memory_struct);
	return result;
}

template<typename T>
T read(uint64_t address)
{
	if (address > 0x7FFFFFFFFFFF || address < 1) return 0;

	T buffer = NULL;

	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 3;
	memory_struct.usermode_pid = usermode_pid;
	memory_struct.target_pid = m_pid;
	memory_struct.address = reinterpret_cast<void*>(address);
	memory_struct.output = &buffer;
	memory_struct.size = sizeof(T);

	int result = call_driver_control(m_driver_control, &memory_struct);

	return result == 0 ? buffer : 1;
}
template<typename T>
bool write(uint64_t address, T buffer)
{
	if (address > 0x7FFFFFFFFFFF || address < 1) return 0;

	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 7;
	memory_struct.usermode_pid = usermode_pid;
	memory_struct.target_pid = m_pid;
	memory_struct.address = reinterpret_cast<void*>(address);
	memory_struct.size = sizeof(T);
	memory_struct.output = &buffer;

	int result = call_driver_control(m_driver_control, &memory_struct);

	if (result != 0)
		return false;

	return true;
}
template<typename T>
std::string lerstring(uint64_t address)
{
	if (address > 0x7FFFFFFFFFFF || address < 1) return 0;

	char buffer[100];

	if (!m_pid)
		return buffer;

	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 3;
	memory_struct.usermode_pid = usermode_pid;
	memory_struct.target_pid = m_pid;
	memory_struct.address = reinterpret_cast<void*>(address);
	memory_struct.size = sizeof(T);
	memory_struct.output = &buffer;

	int result = call_driver_control(m_driver_control, &memory_struct);

	if (result != 0)
		return "";

	std::string nameString;
	for (int i = 0; i < 100; i++) {
		if (buffer[i] == 0)
			break;
		else
			nameString += buffer[i];
	};

	return nameString;
}
template<typename T>
T readVec(uint64_t address)
{
	if (address > 0x7FFFFFFFFFFF || address < 1);

	Vector buffer{};

	if (!m_pid)
		return buffer;

	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 3;
	memory_struct.usermode_pid = usermode_pid;
	memory_struct.target_pid = m_pid;
	memory_struct.address = reinterpret_cast<void*>(address);
	memory_struct.size = sizeof(T);
	memory_struct.output = &buffer;

	int result = call_driver_control(m_driver_control, &memory_struct);

	if (result != 0)
		return Vector{};

	return buffer;
}
template<typename T>
T readF(uint64_t address)
{
	if (address > 0x7FFFFFFFFFFF || address < 1);

	FTransform buffer{};

	if (!m_pid)
		return buffer;

	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 3;
	memory_struct.usermode_pid = usermode_pid;
	memory_struct.target_pid = m_pid;
	memory_struct.address = reinterpret_cast<void*>(address);
	memory_struct.size = sizeof(T);
	memory_struct.output = &buffer;

	int result = call_driver_control(m_driver_control, &memory_struct);

	if (result != 0)
		return FTransform{};

	return buffer;
}
std::string read_str(uintptr_t dw_address, uintptr_t dw_size)
{
	if (dw_address > 0x7FFFFFFFFFFF || dw_address < 1);

	DWORD bytes;
	std::vector<char> buffer(dw_size, char{ 0 });

	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 3;
	memory_struct.usermode_pid = usermode_pid;
	memory_struct.target_pid = m_pid;
	memory_struct.address = reinterpret_cast<void*>(dw_address);
	memory_struct.output = (ULONGLONG*)& buffer[0];
	memory_struct.size = dw_size;

	int result = call_driver_control(m_driver_control, &memory_struct);

	if (result != 0)
		return std::string{};

	return std::string(buffer.data());
}
std::wstring read_wchar(uintptr_t dw_address, uintptr_t dw_size)
{
	if (dw_address > 0x7FFFFFFFFFFF || dw_address < 1);

	DWORD bytes;
	std::vector<wchar_t> buffer(dw_size, char{ 0 });

	MEMORY_STRUCT2 memory_struct = { 0 };
	memory_struct.type = 3;
	memory_struct.usermode_pid = usermode_pid;
	memory_struct.target_pid = m_pid;
	memory_struct.address = reinterpret_cast<void*>(dw_address);
	memory_struct.output = (ULONGLONG*)& buffer[0];
	memory_struct.size = dw_size;

	int result = call_driver_control(m_driver_control, &memory_struct);

	if (result != 0)
		return std::wstring{};

	return std::wstring(buffer.data());
}

#define READ32(addr) read<uint32_t>(addr)
#define READ64(addr) read<uint64_t>(addr)
#define RPMDPTR(addr)   read<DWORD_PTR>(addr)
#define ReadInteger(addr)  read<int>(addr)
#define ReadFloat(addr)  read<float>(addr)
#define READULONG64(addr)  read<ULONG64>(addr)
#define READV(addr) readVec<Vector>(addr)
#define RPMFT(addr)   readF<FTransform>(addr)
#define readchar(addr)   read<lerstring<std::string>(addr)
