#include "skyline/inlinehook/And64InlineHook.hpp"
#include "skyline/utils/cpputils.hpp"
#include "skyline/inlinehook/memcpy_controlled.hpp"

extern "C" {
	uint64_t _ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEaSERKS5_(void* string_to, const char* string_from);
	uint64_t _ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc(void* string_to, const char* string_from);
	uint64_t _ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(void* string_to, const char* string_from);
}

struct Translation {
	const char* ENG;
	const char* JPN;
};

std::array systemStrings = {
	Translation("Saved at slot %d!", "%d番にセーブしました"),
	Translation("Quick Save %d", "QUICK %d"), //Bigger string allowed
	Translation("Auto Save %d", "AUTO %d"), //Bigger string allowed
	Translation("Save %d", "%d"), //Bigger string allowed
	Translation(" has been loaded!", "番をロードしました"),
	Translation("Did a Quick Jump!", "クイックジャンプしました"),
	Translation("Jumped to previous selection!", "前の選択肢に戻りました"),
	Translation(" is empty!", "番にはセーブされていません"),

	//0x3A169C
	Translation("Jump not allowed at selection!", "選択肢ではジャンプできません"),
	Translation("Cannot scroll further!", "これ以上スクロールできません"),
	Translation("Cannot go back further!", "これ以上戻れません"),
	Translation("Backlog doesn't exist!", "バックログが存在しません"),
	Translation("Cannot jump to unread sentences!", "未読文はジャンプできません"),
	Translation("Cannot go back here!", "ここには戻れません"),
	Translation("Cannot skip unread texts!", "未読文はスキップできません"),
	Translation("Cannot be used unless cleared!", "クリアしないと使用できません"),
	Translation("Action not possible at selection!", "選択肢では使用できません"),

	//0x3A15D8
	Translation("Select save to edit comment!", "コメントを編集するデータを選択してください"),

	//0x3A1558 + 0x3D4120
	Translation("Select save to move!", "移動（入れ替え）するデータを選択してください"),

	//0x3D273C
	Translation("Quick saved!", "クイックセーブしました"),

	//0x3A0B84
	Translation("Select save to delete!", "削除するデータを選択してください"),

	//0x3A5A5C
	Translation("This save is protected!", "このデータは保護されています"),

	//0x3A63CC
	Translation("Select where to move save!", "移動（入れ替え）先のデータを選択してください"),

	//0x3E6538
	Translation("Autosaved!", "オートセーブしました"),

	//0x4315F8 
	Translation("@nPress Y@nto protect save", "@nＹボタンで@nデータ保護"),

	//0x431648
	Translation("@nPress Y@nto remove save protection", "解除"),

	//0x43161C
	Translation("@nAutosaves ", "@nオートセーブ"),

	//0x431630
	Translation("@nQuick saves ", "@nクイックセーブ"),

	//0x431644
	Translation("@nare always protected.", "データは@n保護されています@n（データの上書き、移動、@n削除、編集は行えません）"),

	//0x432500 29
	Translation("You cannot remove protection from this save!", "このデータの保護は解除できません")
};

uintptr_t TextRegionOffset = 0;

ptrdiff_t returnInstructionOffset(uintptr_t LR) {
	return LR - TextRegionOffset;
}

struct long_string {
	uint64_t unk;
	uint64_t string_size;
	char* string;
};

uint16_t countLongestLine(const char* string) {
	size_t bytes_count = strlen(string);
	uint32_t i = 0;
	size_t ret_character_count = 0;
	size_t character_count = 0;

	while(i < bytes_count) {
		if (string[i] == 0x40) { //@
			if (string[i+1] == 0x6E) { //n
				if (character_count > ret_character_count)
					ret_character_count = character_count;
				character_count = 0;
				i += 2;
				continue;
			}
			i += 2;
		}
		else if (string[i] < 0xC2) {
			i += 1;
		}
		else if (string[i] < 0xE0) {
			i += 2;
		}
		else if (string[i] < 0xF0) {
			i += 3;
		}
		else i += 4;
		character_count += 1;
	}
	if (character_count > ret_character_count)
		ret_character_count = character_count;

	return ret_character_count;
}

uint8_t countLines(const char* string) {
	size_t bytes_count = strlen(string);
	uint32_t i = 0;
	uint8_t line_count = 1;

	while(i < bytes_count) {
		if (string[i] == 0x40) { //@
			if (string[i+1] == 0x6E) { //n
				line_count += 1;
			}
			i += 2;
		}
		else if (string[i] < 0xC2) {
			i += 1;
		}
		else if (string[i] < 0xE0) {
			i += 2;
		}
		else if (string[i] < 0xF0) {
			i += 3;
		}
		else i += 4;
	}

	return line_count;
}

uint32_t replaceTextForBubbleRender(char* string) {
	uint16_t width = countLongestLine(string);
	width = (width / 2) + (width % 2);
	uint8_t countedLines = countLines(string);
	memset(string, 0x41, width);
	/* Game supports max 3 lines in message bubble, 
	for more lines it will render bubble as for 3 lines and text will go out of bounds. */
	for (int i = 1; i < countedLines; i++) {
		memcpy(&string[width], "@nA", 3);
		width += 3;
	}
	string[width] = 0;
	return width;
}

uint64_t (*std_string_operator_equal_original)(void* string_to, char* string_from);
uint64_t std_string_operator_equal_hook(void* string_to, char* string_from) {
	ptrdiff_t offset = returnInstructionOffset((uintptr_t)__builtin_return_address(0));

	if (offset == 0x11668) {
		if (!std::char_traits<char>::compare(&string_from[strlen(string_from) - 1], "\"", 1)) {
			*(uint8_t*)string_from = replaceTextForBubbleRender(&string_from[1]) * 2;
		}
		else {
			long_string* buffer = (long_string*)string_from;
			if ((strlen(string_from) == 1) &&
			 	(!std::char_traits<char>::compare(&(buffer -> string[(buffer -> string_size) - 1]), "\"", 1))) {
					buffer -> string_size = replaceTextForBubbleRender(buffer -> string);
			}
		}
	}

	return std_string_operator_equal_original(string_to, string_from);
}

uint64_t (*std_string_append_original)(void* string_to, char* string_from);
uint64_t std_string_append_hook(void* string_to, char* string_from) {
	ptrdiff_t offset = returnInstructionOffset((uintptr_t)__builtin_return_address(0));

	if (offset == 0x431650) {
		if (!memcmp(string_from, systemStrings[25].JPN, strlen(systemStrings[25].JPN) + 1)) {
			return _ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc(string_to, systemStrings[25].ENG);
		}
	}

	return std_string_append_original(string_to, string_from);
}

struct textStruct {
	const uint64_t struct_data_size;
	uint64_t utf8_size;
	char* original_text;
	const void* unk;
};

void (*convertUtf8Text_original)(textStruct* text_Struct, const uint32_t unk1, const uint32_t unk2);
void convertUtf8Text_hook(textStruct* text_Struct, const uint32_t unk1, const uint32_t unk2) {
	ptrdiff_t offset = returnInstructionOffset((uintptr_t)__builtin_return_address(0));

	switch(offset) {
		case 0x3A169C:
			for (int i = 8; i < 17; i++) {
				if (!memcmp(text_Struct -> original_text, systemStrings[i].JPN, strlen(text_Struct -> original_text) + 1)) {
					memcpy(text_Struct -> original_text, systemStrings[i].ENG, strlen(systemStrings[i].ENG) + 1);
					text_Struct -> utf8_size = strlen(systemStrings[i].ENG);
					return convertUtf8Text_original(text_Struct, unk1, unk2);
				}			
			}
			break;
		case 0x3A15D8:
			if (!memcmp(text_Struct -> original_text, systemStrings[17].JPN, strlen(text_Struct -> original_text) + 1)) {
				memcpy(text_Struct -> original_text, systemStrings[17].ENG, strlen(systemStrings[17].ENG) + 1);
				text_Struct -> utf8_size = strlen(systemStrings[17].ENG);
			}
			break;
		case 0x3A1558:
		case 0x3D4120:
			if (!memcmp(text_Struct -> original_text, systemStrings[18].JPN, strlen(text_Struct -> original_text) + 1)) {
				memcpy(text_Struct -> original_text, systemStrings[18].ENG, strlen(systemStrings[18].ENG) + 1);
				text_Struct -> utf8_size = strlen(systemStrings[18].ENG);
			}
			break;
		case 0x3D273C:
			if (!memcmp(text_Struct -> original_text, systemStrings[19].JPN, strlen(text_Struct -> original_text) + 1)) {
				memcpy(text_Struct -> original_text, systemStrings[19].ENG, strlen(systemStrings[19].ENG) + 1);
				text_Struct -> utf8_size = strlen(systemStrings[19].ENG);
			}
			break;
		case 0x3A0B84:
			if (!memcmp(text_Struct -> original_text, systemStrings[20].JPN, strlen(text_Struct -> original_text) + 1)) {
				memcpy(text_Struct -> original_text, systemStrings[20].ENG, strlen(systemStrings[20].ENG) + 1);
				text_Struct -> utf8_size = strlen(systemStrings[20].ENG);
			}
			break;
		case 0x3A5A5C:
			if (!memcmp(text_Struct -> original_text, systemStrings[21].JPN, strlen(text_Struct -> original_text) + 1)) {
				memcpy(text_Struct -> original_text, systemStrings[21].ENG, strlen(systemStrings[21].ENG) + 1);
				text_Struct -> utf8_size = strlen(systemStrings[21].ENG);
			}
			break;
		case 0x3A63CC:
			if (!memcmp(text_Struct -> original_text, systemStrings[22].JPN, strlen(text_Struct -> original_text) + 1)) {
				memcpy(text_Struct -> original_text, systemStrings[22].ENG, strlen(systemStrings[22].ENG) + 1);
				text_Struct -> utf8_size = strlen(systemStrings[22].ENG);
			}
			break;
		case 0x3E6538:
			if (!memcmp(text_Struct -> original_text, systemStrings[23].JPN, strlen(text_Struct -> original_text) + 1)) {
				memcpy(text_Struct -> original_text, systemStrings[23].ENG, strlen(systemStrings[23].ENG) + 1);
				text_Struct -> utf8_size = strlen(systemStrings[23].ENG);
			}
			break;
		case 0x432500:
			if (!memcmp(text_Struct -> original_text, systemStrings[29].JPN, strlen(text_Struct -> original_text) + 1)) {
				memcpy(text_Struct -> original_text, systemStrings[29].ENG, strlen(systemStrings[29].ENG) + 1);
				text_Struct -> utf8_size = strlen(systemStrings[29].ENG);
			}
			break;
	}
	return convertUtf8Text_original(text_Struct, unk1, unk2);
}

void (*convertUtf8Text2_original)(textStruct* _textStruct, const char* format, uint32_t number);
void convertUtf8Text2_hook(textStruct* _textStruct, const char* format, uint32_t number) {
	ptrdiff_t offset = returnInstructionOffset((uintptr_t)__builtin_return_address(0));

	switch(offset) {
		case 0x3D4270:
		case 0x3A6498:
			if (!strcmp(systemStrings[1].JPN, format)) {
				format = systemStrings[1].ENG;
			}
			else if (!strcmp(systemStrings[2].JPN, format)) {
				format = systemStrings[2].ENG;
			}
			else if (!strcmp(systemStrings[3].JPN, format)) {
				format = systemStrings[3].ENG;
			}
			break;
	}
	return convertUtf8Text2_original(_textStruct, format, number);
}

void AL_main()
{
	TextRegionOffset = (uintptr_t)skyline::utils::getRegionAddress(skyline::utils::region::Text);

	A64HookFunction((void*)(_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEaSERKS5_), 
		reinterpret_cast<void*>(std_string_operator_equal_hook), (void**)&std_string_operator_equal_original);

	A64HookFunction((void*)(_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc), 
		reinterpret_cast<void*>(std_string_append_hook), (void**)&std_string_append_original);

	A64HookFunction((void*)(TextRegionOffset + 0x40CAB4), 
		reinterpret_cast<void*>(convertUtf8Text_hook), (void**)&convertUtf8Text_original);

	A64HookFunction((void*)(TextRegionOffset + 0x382210), 
		reinterpret_cast<void*>(convertUtf8Text2_hook), (void**)&convertUtf8Text2_original);
	
	sky_memcpy((void*)(TextRegionOffset+0x4DF146), systemStrings[0].ENG, strlen(systemStrings[0].ENG) + 1);
	sky_memcpy((void*)(TextRegionOffset+0x4DAFD5), systemStrings[4].ENG, strlen(systemStrings[4].ENG) + 1);
	sky_memcpy((void*)(TextRegionOffset+0x4DB491), systemStrings[5].ENG, strlen(systemStrings[5].ENG) + 1);
	sky_memcpy((void*)(TextRegionOffset+0x4DCCBD), systemStrings[6].ENG, strlen(systemStrings[6].ENG) + 1);
	sky_memcpy((void*)(TextRegionOffset+0x4DDA83), systemStrings[7].ENG, strlen(systemStrings[7].ENG) + 1);
	sky_memcpy((void*)(TextRegionOffset+0x4D8D42), systemStrings[24].ENG, strlen(systemStrings[24].ENG) + 1);
	sky_memcpy((void*)(TextRegionOffset+0x4DD580), systemStrings[26].ENG, strlen(systemStrings[26].ENG) + 1);
	sky_memcpy((void*)(TextRegionOffset+0x4D7D13), systemStrings[27].ENG, strlen(systemStrings[27].ENG) + 1);
	sky_memcpy((void*)(TextRegionOffset+0x4DFA95), systemStrings[28].ENG, strlen(systemStrings[28].ENG) + 1);
}