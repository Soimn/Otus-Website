#include <stdio.h>
#include <windows.h>

typedef struct String
{
    char* data;
    int size;
} String;

typedef struct Dev_Log_Entry
{
    struct Dev_Log_Entry* next;
    char* id;
} Dev_Log_Entry;

unsigned char
IsAlpha(char c)
{
    return (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z');
}

unsigned char
IsDigit(char c)
{
    return (c >= '0' && c <= '9');
}

unsigned char
IsNewline(char c)
{
    return (c == '\r' || c == '\n');
}

unsigned char
IsWhitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\v');
}

unsigned char
ToLower(char c)
{
    return (c >= 'A' && c <= 'Z' ? (c - 'A') + 'a' : c);
}

unsigned char
ToUpper(char c)
{
    return (c >= 'a' && c <= 'z' ? (c - 'a') + 'A' : c);
}

unsigned char
StringCStringCompare(String string, const char* cstring)
{
    while (string.size && *cstring && *string.data == *cstring)
    {
        string.data += 1;
        string.size -= 1;
        cstring      = (char*)cstring + 1;
    }
    
    return (string.size == 0 && *cstring == 0);
}

unsigned char
IsKeyword(String string)
{
    return (StringCStringCompare(string, "proc")     ||
            StringCStringCompare(string, "struct")   ||
            StringCStringCompare(string, "enum")     ||
            StringCStringCompare(string, "if")       ||
            StringCStringCompare(string, "else")     ||
            StringCStringCompare(string, "defer")    ||
            StringCStringCompare(string, "for")      ||
            StringCStringCompare(string, "while")    ||
            StringCStringCompare(string, "do")       ||
            StringCStringCompare(string, "break")    ||
            StringCStringCompare(string, "continue") ||
            StringCStringCompare(string, "import")   ||
            StringCStringCompare(string, "return")   ||
            StringCStringCompare(string, "int")      ||
            StringCStringCompare(string, "float")    ||
            StringCStringCompare(string, "bool")     ||
            StringCStringCompare(string, "I8")       ||
            StringCStringCompare(string, "I16")      ||
            StringCStringCompare(string, "I32")      ||
            StringCStringCompare(string, "I64")      ||
            StringCStringCompare(string, "U8")       ||
            StringCStringCompare(string, "U16")      ||
            StringCStringCompare(string, "U32")      ||
            StringCStringCompare(string, "U64")      ||
            StringCStringCompare(string, "B8")       ||
            StringCStringCompare(string, "B16")      ||
            StringCStringCompare(string, "B32")      ||
            StringCStringCompare(string, "B64")      ||
            StringCStringCompare(string, "F32")      ||
            StringCStringCompare(string, "F64")      ||
            StringCStringCompare(string, "byte")     ||
            StringCStringCompare(string, "word")     ||
            StringCStringCompare(string, "rawptr")   ||
            StringCStringCompare(string, "typeid"));
}

void
ParseMarkup(char* post_file_name, char* start, unsigned long length, FILE* output)
{
    char* cursor        = start;
    unsigned long space = length;
    
    do
    {
        while (space && (IsWhitespace(*cursor) || IsNewline(*cursor)))
        {
            cursor += 1;
            space  -= 1;
        }
        
        unsigned char is_italic    = 0;
        unsigned char is_bold      = 0;
        unsigned char is_underline = 0;
        for (;;)
        {
            if (space && (IsWhitespace(cursor[0]) || IsNewline(cursor[0])))
            {
                while (space && (IsWhitespace(cursor[0]) || IsNewline(cursor[0])))
                {
                    cursor += 1;
                    space  -= 1;
                }
                
                putc(' ', output);
            }
            
            if (space == 0 || *cursor == 0) break;
            else
            {
                if (*cursor == '\\')
                {
                    if (space > 1)
                    {
                        cursor += 1;
                        space  -= 1;
                        
                        if      (*cursor == 'n')  fprintf(output, "<br>");
                        else if (*cursor == 't')  fprintf(output, "<span class=\"whitespace\">    </span>");
                        else if (*cursor == 's')  fprintf(output, "<span class=\"whitespace\"> </span>");
                        else if (*cursor != ' ')  putc(*cursor, output);
                        
                        cursor += 1;
                        space  -= 1;
                    }
                    
                    else
                    {
                        putc(*cursor, output);
                    }
                }
                
                else if (*cursor == '*')
                {
                    cursor += 1;
                    space  -= 1;
                    
                    if (space && cursor[0] == '*')
                    {
                        cursor += 1;
                        space  -= 1;
                        
                        is_bold = !is_bold;
                        fprintf(output, "<%sb>", (is_bold ? "" : "/"));
                    }
                    
                    else
                    {
                        is_italic = !is_italic;
                        fprintf(output, "<%si>", (is_italic ? "" : "/"));
                    }
                }
                
                else if (*cursor == '_' && (is_underline || (cursor == start || IsWhitespace(*(cursor - 1)))))
                {
                    cursor += 1;
                    space  -= 1;
                    
                    is_underline = !is_underline;
                    fprintf(output, "<%su>", (is_underline ? "" : "/"));
                }
                
				else if (space > 1 && cursor[0] == '!' && cursor[1] == '(')
				{
					cursor += 2;
					space  -= 2;
                    
					String subst = { .data = cursor, .size = 0 };
					while (space != 0)
					{
                        cursor += 1;
                        space  -= 1;
                        
                        if (cursor[-1] != ',') subst.size += 1;
                        else break;
					}
                    
					String disp_text = { .data = cursor, .size = 0 };
					while (space != 0 && cursor[0] != ')')
					{
                        cursor += 1;
                        space  -= 1;
                        disp_text.size += 1;
					}
                    
					int did_fail = 0;
					char* url    = 0;
					if (space != 0)
					{
						cursor += 1;
                        
                        
						if (StringCStringCompare(subst, "jai_vids") || StringCStringCompare(subst, "jai"))
						{
						    url = "https://www.youtube.com/playlist?list=PLmV5I2fxaiCKfxMBrNsU1kgKJXD3PkyxO";
						}
                        
						else if (StringCStringCompare(subst, "odin"))
						{
							url = "https://odin-lang.org/";
						}
                        
                        else if (StringCStringCompare(subst, "zig"))
                        {
                            url = "https://ziglang.org/";
                        }
                        
						else did_fail = 1;
					}
                    
					else did_fail = 1;
                    
					if (did_fail)
					{
						fprintf(output, "<b color=\"red\">FAILED TO SUBSTITUTE EXPRESSION<b>");
					}
                    
					else
					{
						fprintf(output, "<a href=\"%s\">", url);
						for (unsigned long long int i = 0; i < disp_text.size; ++i) putc(disp_text.data[i], output);
						fprintf(output, "</a>");
					}
				}
                
                else if (space > 2 && cursor[0] == '`' && cursor[1] == '`' && cursor[2] == '`')
                {
                    cursor += 3;
                    space  -= 3;
                    
                    fprintf(output, "\n<div class=\"code\">\n");
                    
                    while (space && (IsWhitespace(*cursor) || IsNewline(*cursor)))
                    {
                        cursor += 1;
                        space  -= 1;
                    }
                    
                    while (space != 0)
                    {
                        while (space && (IsWhitespace(*cursor) || IsNewline(*cursor)))
                        {
                            putc(*cursor, output);
                            
                            cursor += 1;
                            space  -= 1;
                        }
                        
                        if (space > 2 && cursor[0] == '`' && cursor[1] == '`' && cursor[2] == '`') break;
                        
                        if (IsAlpha(*cursor))
                        {
                            String identifier = {
                                .data = cursor,
                                .size = 0
                            };
                            
                            while (space && (IsAlpha(*cursor) || IsDigit(*cursor) || *cursor == '_'))
                            {
                                identifier.size += 1;
                                
                                cursor += 1;
                                space  -= 1;
                            }
                            
                            unsigned char is_keyword = IsKeyword(identifier);
                            if (is_keyword)
                            {
                                fprintf(output, "<span class=\"code_keyword\">");
                            }
                            
                            for (unsigned int i = 0; i < identifier.size; ++i)
                            {
                                putc(identifier.data[i], output);
                            }
                            
                            if (is_keyword)
                            {
                                fprintf(output, "</span>");
                            }
                            
                        }
                        
                        // This does not handle // /*
                        else if (space > 1 && cursor[0] == '/' && cursor[1] == '/')
                        {
                            fprintf(output, "<span class=\"code_comment\">");
                            
                            while (space && !IsNewline(*cursor))
                            {
                                putc(*cursor, output);
                                
                                cursor += 1;
                                space  -= 1;
                            }
                            
                            fprintf(output, "</span>");
                        }
                        
                        else if (space > 1 && cursor[0] == '/' && cursor[1] == '*')
                        {
                            fprintf(output, "<span class=\"code_comment\">");
                            
                            unsigned int nest_level = 1;
                            while (space && nest_level != 0)
                            {
                                if (space > 1 && cursor[0] == '/' && cursor[1] == '*')
                                {
                                    nest_level += 1;
                                    
                                    putc(*cursor, output);
                                    cursor += 2;
                                    space  -= 2;
                                }
                                
                                else if (space > 1 && cursor[0] == '*' && cursor[1] == '/')
                                {
                                    nest_level -= 1;
                                    
                                    putc(*cursor, output);
                                    cursor += 2;
                                    space  -= 2;
                                }
                                
                                else
                                {
                                    putc(*cursor, output);
                                    cursor += 1;
                                    space  -= 1;
                                }
                            }
                            
                            fprintf(output, "</span>");
                        }
                        
                        else if (*cursor == '<' || *cursor == '>')
                        {
                            fprintf(output, (*cursor == '<' ? "&lt;" : "&gt;"));
                            
                            cursor += 1;
                            space  -= 1;
                        }
                        
                        else
                        {
                            putc(*cursor, output);
                            
                            cursor += 1;
                            space  -= 1;
                        }
                    }
                    
                    if (space != 0)
                    {
                        cursor += 3;
                        space  -= 3;
                    }
                    
                    fprintf(output, "</div>\n");
                }
                
                else
                {
                    putc(*cursor, output);
                    
                    cursor += 1;
                    space  -= 1;
                }
            }
        }
    } while (0);
}

int
main(const int argc, const char** argv)
{
    do
    {
        FILE* output = fopen("index.html", "wb");
        
        if (output == 0)
        {
            fprintf(stderr, "ERROR: Failed create resulting html file");
            break;
        }
        
        FILE* script_output = fopen("generated_script.js", "wb");
        
        if (script_output == 0)
        {
            fprintf(stderr, "ERROR: Failed create resulting script file");
            break;
        }
        
        String about      = {0};
        String stylesheet = {0};
        String script     = {0};
        
        { /// Load About
            
            HANDLE file_handle = CreateFileA("./about.txt", GENERIC_READ, FILE_SHARE_READ,
                                             0, OPEN_EXISTING, 0, 0);
            
            about.size = GetFileSize(file_handle, 0);
            about.data = malloc(about.size);
            
            unsigned long bytes_read = 0;
            if (about.size == INVALID_FILE_SIZE                                ||
                !ReadFile(file_handle, about.data, about.size, &bytes_read, 0) ||
                bytes_read != about.size)
            {
                fprintf(stderr, "Failed to read about text");
                break;
            }
        }
        
        { /// Script
            HANDLE file_handle = CreateFileA("./script.js", GENERIC_READ, FILE_SHARE_READ,
                                             0, OPEN_EXISTING, 0, 0);
            
            script.size = GetFileSize(file_handle, 0);
            script.data = malloc(script.size);
            
            unsigned long bytes_read = 0;
            if (script.size == 0                                                 ||
                !ReadFile(file_handle, script.data, script.size, &bytes_read, 0) ||
                bytes_read != script.size)
            {
                fprintf(stderr, "Failed to read script");
                break;
            }
        }
        
        fprintf(output, "<!doctype html>\n<html lang=\"en\">\n<head>\n");
        
        fprintf(output, "<title>Otus Programming Language</title>\n");
        fprintf(output, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
        fprintf(output, "<meta charset=\"utf-8\"/>\n");
        
        fprintf(output, "<meta name=\"description\" content=\"");
        fprintf(output, "Documentation and useful information about the Otus systems programming language. ");
		fprintf(output, "The Otus language aims to be low level and &quot;simple but powerful&quot;\"/>\n");
        
        fprintf(output, "<meta name=\"author\" content=\"Simon Doksrød\"/>");
        fprintf(output, "<meta name=\"robots\" content=\"index,follow\">");
        
        fprintf(output, "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" title=\"main\">\n");
        fprintf(output, "<script src=\"generated_script.js\"></script>\n");
        
        fprintf(output, "</head>\n");
        fprintf(output, "\n<body>\n");
        
        fprintf(output, "<div id=\"page_container\">\n");
        fprintf(output, "<div id=\"header\">\n");
        fprintf(output, "<img src=\"Otus.svg\" alt=\"Otus logo\" id=\"logo\"/>\n");
        fprintf(output, "<ul id=\"navbar\">\n");
        fprintf(output, "<li><a href=\"#about\"><b>About</b></a></li><li class=\"navbar_vl\">|</li>\n");
        fprintf(output, "<li><a href=\"#docs\"><b>Docs</b></a></li><li class=\"navbar_vl\">|</li>\n");
        fprintf(output, "<li><a href=\"#log\"><b>Dev Log</b></a></li><li class=\"navbar_vl\">|</li>\n");
        fprintf(output, "<li><a href=\"https://github.com/Soimn/Otus\"><b>GitHub</b></a></li>\n");
        fprintf(output, "</ul>\n</div>\n");
        
        fprintf(output, "\n<div id=\"content\">\n");
        
        /// About tab
        
        fprintf(output, "\n<div id=\"about_tab\">\n");
        
        ParseMarkup("about.txt", about.data, about.size, output);
        
        free(about.data);
        about.data = 0;
        about.size = 0;
        
        fprintf(output, "</div>\n");
        
        
        /// Docs tab
        
        fprintf(output, "\n<div id=\"docs_tab\">\n");
        fprintf(output, "</div>\n");
        
        /// Dev log tab
        
        // NOTE(soimn): list of all log post ids
        unsigned int dev_log_id_count    = 0;
        unsigned int dev_log_id_capacity = 0;
        char** dev_log_ids               = malloc(dev_log_id_capacity * sizeof(char*));
        
        fprintf(output, "\n<div id=\"log_tab\">\n");
        
        fprintf(output, "<div id=\"log_intro\">\n");
        fprintf(output, "This is a collection of articles in a log-esque format that try to describe the problems I have encountered while designing the language and compiler.");
        fprintf(output, " The first post was written about half a year after the start of development, as to not hamstring the development by writing articles instead of code.");
        fprintf(output, "\n</div>\n");
        
        if (argc == 2)
        {
            String search_path = {0};
            {
                String dir_path = {
                    .data = (char*)argv[argc - 1],
                    .size = strlen(argv[argc - 1])
                };
                
                if (dir_path.data[dir_path.size - 1] == '/' ||
                    dir_path.data[dir_path.size - 1] == '\\')
                {
                    dir_path.size -= 1;
                }
                
                search_path.size = dir_path.size + (sizeof("/*.post") - 1);
                search_path.data = malloc(search_path.size + 1);
                
                memcpy(search_path.data, dir_path.data, dir_path.size);
                memcpy(search_path.data + dir_path.size, "/*.post", sizeof("/*.post") - 1);
                
                search_path.data[search_path.size] = 0;
            }
            
            WIN32_FIND_DATA find_data = {0};
            HANDLE search_handle = FindFirstFileA(search_path.data, &find_data);
            
            if (search_handle != INVALID_HANDLE_VALUE)
            {
                unsigned long file_buffer_size = 64 * 1024 * 1024;
                char* file_buffer              = malloc(file_buffer_size);
                
                do
                {
                    char* file_name = find_data.cFileName;
                    unsigned long file_size = find_data.nFileSizeLow;
                    
                    char* file_path      = 0;
                    {
                        unsigned long file_name_size = strlen(file_name);
                        unsigned long dir_path_size  = search_path.size - (sizeof("*.post") - 1);
                        
                        file_path = malloc(dir_path_size + file_name_size + 1);
                        
                        memcpy(file_path, search_path.data, dir_path_size);
                        memcpy(file_path + dir_path_size, file_name, file_name_size);
                        file_path[dir_path_size + file_name_size] = 0;
                    }
                    
                    if (find_data.nFileSizeHigh != 0)
                    {
                        fprintf(stderr, "ERROR: File too large. Skipping the post file \"%s\"", file_name);
                        continue;
                    }
                    
                    if (file_buffer_size < file_size)
                    {
                        free(file_buffer);
                        
                        file_buffer_size = file_size;
                        file_buffer      = malloc(file_buffer_size + 1);
                    }
                    
                    HANDLE file_handle = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ,
                                                     0, OPEN_EXISTING, 0, 0);
                    
                    unsigned long bytes_read = 0;
                    if (ReadFile(file_handle, file_buffer, file_size, &bytes_read, 0) &&
                        bytes_read == file_size)
                    {
                        char* cursor       = file_buffer;
                        unsigned int space = file_size;
                        
                        cursor[space] = 0;
                        
                        do
                        {
                            if (space < sizeof("xx/xx/xx") - 1)
                            {
                                fprintf(stderr, "ERROR: Missing date in \"%s\"", file_name);
                                break;
                            }
                            
                            unsigned long date_size = sizeof("xx/xx/xx") - 1;
                            char* date              = cursor;
                            
                            space  -= date_size;
                            cursor += date_size;
                            
                            unsigned int day, month, year = 0;
                            if (IsDigit(date[0]) && IsDigit(date[1]) && date[2] == '/' &&
                                IsDigit(date[3]) && IsDigit(date[4]) && date[5] == '/' &&
                                IsDigit(date[6]) && IsDigit(date[7]))
                            {
                                day   = (date[0] - '0') * 10 + (date[1] - '0');
                                month = (date[3] - '0') * 10 + (date[4] - '0');
                                year  = (date[6] - '0') * 10 + (date[7] - '0');
                            }
                            
                            else
                            {
                                fprintf(stderr, "ERROR: Invalid date format. Date format \"dd/mm/yy\"");
                                break;
                            }
                            
                            if (space && *cursor == '\r') cursor += 1, space -= 1;
                            if (space && *cursor == '\n')
                            {
                                cursor += 1;
                                space  -= 1;
                            }
                            
                            else
                            {
                                fprintf(stderr, "ERROR: Missing newline after date");
                                break;
                            }
                            
                            char* raw_title = cursor;
                            while (space && !IsNewline(*cursor))
                            {
                                cursor += 1;
                                space  -= 1;
                            }
                            
                            unsigned raw_title_size = cursor - raw_title;
                            
                            while (raw_title_size != 0 && IsWhitespace(*raw_title))
                            {
                                raw_title    += 1;
                                raw_title_size -= 1;
                            }
                            
                            String title = {
                                .data = malloc(raw_title_size + 1),
                                .size = 0
                            };
                            
                            String log_id = {
                                .data = malloc(raw_title_size + (sizeof("yymmdd") - 1) + 1),
                                .size = 0
                            };
                            
                            log_id.size += 1;
                            log_id.data[log_id.size - 1] = ToLower(raw_title[0]);
                            
                            for (unsigned int i = 0, last_was_space = 0; i < raw_title_size; ++i)
                            {
                                if (IsWhitespace(raw_title[i])) last_was_space = 1;
                                else
                                {
                                    if (last_was_space == 1)
                                    {
                                        title.size += 1;
                                        title.data[title.size - 1] = ' ';
                                        
                                        
                                        log_id.size += 1;
                                        log_id.data[log_id.size - 1] = ToLower(raw_title[i]);
                                        
                                        last_was_space = 0;
                                    }
                                    
                                    title.size += 1;
                                    title.data[title.size - 1] = raw_title[i];
                                }
                            }
                            
                            log_id.size += sprintf(log_id.data + log_id.size, "%.2d%.2d%.2d", year, month, day);
                            
                            title.data[title.size]   = 0;
                            log_id.data[log_id.size] = 0;
                            
                            if (space && *cursor == '\r') cursor += 1, space  -= 1;
                            if (space && *cursor == '\n')
                            {
                                cursor += 1;
                                space  -= 1;
                            }
                            
                            else
                            {
                                fprintf(stderr, "ERROR: Missing newline after title");
                                break;
                            }
                            
                            
                            fprintf(output, "<div class=\"log_title_minimized %s\" onclick=\"window.location.hash=\'#log@%s\'\"><a href=\"#log@%s\">%s</a></div>",
                                    log_id.data, log_id.data, log_id.data, title.data);
                            
                            fprintf(output, "<div class=\"log_date_minimized %s\">%.4d/%.2d/%.2d</div>\n",
                                    log_id.data, 2000 + year, month, day);
                            
                            
                            fprintf(output, "<div class=\"log_content_minimized %s\">\n", log_id.data);
                            
                            ParseMarkup(file_name, cursor, space, output);
                            
                            fprintf(output, "</div>\n");
                            free(title.data);
                            
                            if (dev_log_id_count == dev_log_id_capacity)
                            {
                                dev_log_id_capacity = (unsigned int)(dev_log_id_capacity * 1.68f);
                                
                                char** new_dev_log_ids = malloc(dev_log_id_capacity * sizeof(char*));
                                
                                memcpy(dev_log_ids, new_dev_log_ids, dev_log_id_count * sizeof(char*));
                                
                                free(dev_log_ids);
                                dev_log_ids = new_dev_log_ids;
                            }
                            
                            dev_log_ids[dev_log_id_count] = log_id.data;
                            dev_log_id_count += 1;
                            
                        } while (0);
                    }
                    
                    else
                    {
                        fprintf(stderr, "ERROR: Failed to open file. Skipping the post file \"%s\"", file_name);
                        continue;
                    }
                    
                    CloseHandle(file_handle);
                } while (FindNextFileA(search_handle, &find_data));
                
                FindClose(search_handle);
            }
            
            else
            {
                fprintf(stderr, "No post files were found in the directory", argv[argc - 1]);
            }
        }
        
        else
        {
            fprintf(stderr, "Invalid number of arguments");
        }
        
        fprintf(output, "</div>\n"); // Close log_tab
        fprintf(output, "</div>\n"); // Close content
        fprintf(output, "</div>\n"); // Close page_container
        fprintf(output, "</body>\n</html>");
        
        /// Generated script
        
        fprintf(script_output, "var DevLogIDs = [");
        
        if (dev_log_id_count != 0)
        {
            fprintf(script_output, "\"%s\"", dev_log_ids[0]);
            
            for (unsigned int i = 1; i < dev_log_id_count; ++i)
            {
                fprintf(script_output, ", \"%s\"", dev_log_ids[i]);
            }
        }
        
        fprintf(script_output, "];\n\n");
        
        for (unsigned int i = 0; i < script.size; ++i) putc(script.data[i], script_output);
        
    } while (0);
    
    return 0;
}
