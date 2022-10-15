#pragma once
// toggle the cursor on or off
enum class CursorType{
	CURSOR_ENABLED,
	CURSOR_DISABLED
};
typedef void(*ToggleCursorFunc)(CursorType newVal);
typedef void(*CloseProgramFunc)(void);