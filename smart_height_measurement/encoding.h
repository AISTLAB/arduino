void OneUTF82Unicode(const char *utf_char, char *unicode_char) {
    //unicode: 0x192->110010010 ,utf8:0xC692->11000110|10010010
    //小端序
    int utf_length = strlen(utf_char);
    //0x3F->00111111
    switch (utf_length) {
    case 1:
        unicode_char[0] = utf_char[0];
        unicode_char[1] = 0;
        break;
    case 2:
        unicode_char[0] = (utf_char[1] & 0x3F) | ((utf_char[0] & 0x3) << 6);
        unicode_char[1] = (utf_char[0] & 0x3C) >> 2;
        break;
    case 3:
        unicode_char[0] = (utf_char[2] & 0x3F) | ((utf_char[1] & 0x3) << 6);
        unicode_char[1] = ((utf_char[1] & 0x3C) >> 2) | ((utf_char[0] & 0xF) << 4);
        break;
    default:
        //assert(0);
        //printf("utf_char length is bigger than 4 unsupported.\n");
        break;
    }
}

char *utf82unicode(const char *utf) {
    char *utf8 = strdup(utf);
    size_t utf8_length = strlen(utf8);
    char *unicode = (char *)malloc(utf8_length * 2 + 2);
    memset(unicode, 0, utf8_length * 2 + 2);
    size_t index = 0;
    size_t start = 0;
    unsigned char temp;
    //10000000=0x80 1110=0xE 110=0x6
    unsigned char flag = 0;
    size_t unicode_index = 0;
    bool is_finded = false;
    while ((temp = utf8[index])) {
        start = index;
        temp = temp >> 4 ;
        if (temp > 0xE) {
            //assert(0);
            //printf("utf8 bigger than 4 byte is not supported.\n");
            break;
        }
        if (temp == 0xE) {
            index += 3;
            is_finded = true;
        }
        temp = temp >> 1;
        if (!is_finded && temp == 0x6) {
            index += 2;
            is_finded = true;
        }
        temp = temp >> 2;
        if (!is_finded && temp == 0x00) {
            index += 1;
            is_finded = true;
        }
        if (index > utf8_length) {
            break;
        }
        flag = utf8[index];
        utf8[index] = 0;
        OneUTF82Unicode(utf8 + start, unicode + unicode_index);
        utf8[index] = flag;
        unicode_index += 2;
        is_finded = false;
    }
    free(utf8);
    int unicode_number = unicode_index / 2;
    return unicode;
}
