// StrCpy() copies one string to another... Fairly simple abstraction to save time.
char* strcpy(char *dest, const char* src)
{
	while (*src)		//while source string isnt null, copy, then increase pointer.
	{
		*dest=*src;
		dest++;
		src++;
	}
}