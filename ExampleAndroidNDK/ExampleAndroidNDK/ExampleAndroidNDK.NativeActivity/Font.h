#ifdef __cplusplus
extern "C"
{
#endif

extern void FontDrawSetScreen(uint32_t *pixels , const int width , const int height , const int stride);
extern void FontDraw(const int xpos , const int ypos , const char *theText);

#ifdef __cplusplus
}
#endif
