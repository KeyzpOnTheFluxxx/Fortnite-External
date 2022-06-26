# Fortnite-External
Simple external cheat for the game fortnite. (seems to be ud on both anti cheat)

for the cheat to work you will need to update the w2s and the camera



fnamepool: 0xCC47500
GWorld: 0xCBF38B8

char* v2 = buff; // rbx
unsigned __int16* v3; // rdi
signed int v4 = nameLength; // edi
unsigned int v5; // eax
unsigned int v6; // er9
unsigned int v7; // er8
__int64 result; // rax
unsigned int v9; // er9
signed int v10; // ecx
 
v5 = read<uint64_t>(moduleBase + 0xCB00868);
v6 = v5;
v7 = v5 ^ 0x9C677CC5;
result = 1041204193 * v5;
v9 = v6 % 0x21;
v10 = 0;
 
do
{
	result = v10++ + v9;
	v7 += result;
	*v2++ ^= v7;
} while (v10 < v4);
