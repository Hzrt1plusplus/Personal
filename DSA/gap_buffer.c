// gap buffer implementation in C 
#include <termios.h>                                                                                                 
#include <unistd.h>                                                                                                  
#include <stdio.h>                                                                                                   
#include <string.h>                                                                                                  
#include <malloc.h>                                                                                                  
#include <uchar.h>                                                                                                   
                                                                                                                     
typedef struct GapBuffer {                                                                                           
                                                                                                                     
        char* m_buffer;                                                                                              
        int left;                                                                                                    
        int right;                                                                                                   
        size_t size;                                                                                                 
                                                                                                                     
} GapBuffer ;                                                                                                        
                                                                                                                     
GapBuffer* create_gap_buffer(size_t initial_size)                                                                    
{                                                                                                                    
        char* buff = (char*)malloc(sizeof(char)*initial_size);                                                       
        GapBuffer* result = (GapBuffer*)malloc(sizeof(GapBuffer));                                                   
        result -> left = 0;                                                                                          
        result -> right = initial_size - 1;                                                                          
        result -> m_buffer = buff;                                                                                   
        result -> size = initial_size;                                                                               
        return result;                                                                                               
}

void destroy_gap_buffer(GapBuffer* buff)                                                                             
{                                                                                                                    
        free(buff -> m_buffer);                                                                                      
        free(buff);                                                                                                  
}                                                                                                                    
                                                                                                                     
void delete_prev(GapBuffer* buff)                                                                                    
{                                                                                                                    
        if (buff -> left > 0) buff -> left--;                                                                        
}                                                                                                                    
                                                                                                                     
void delete_next(GapBuffer* buff)                                                                                    
{                                                                                                                    
        if (buff -> right < (buff -> size - 1)) buff -> right++;                                                     
} 

void move_gap_to(GapBuffer* buffer, int pos)                                                                         
{                                                                                                                    
                                                                                                                     
        char* b = buffer -> m_buffer;                                                                                
        while (buffer -> left > pos) {                                                                               
                b[buffer -> right--] = b[--buffer -> left];                                                          
        }                                                                                                            
        while (buffer -> right < pos) {                                                                              
                b[buffer -> left++] = b[++buffer -> right];                                                          
        }                                                                                                            
}                                                                                                                    
                                                                                                                     
void expand_gap(GapBuffer* buffer)                                                                                   
{                                                                                                                    
        size_t new_size = buffer -> size * 2;                                                                        
        char* new_buffer = (char*) malloc(new_size * sizeof(char));                                                  
                                                                                                                     
        memcpy(new_buffer, buffer -> m_buffer, buffer -> left * sizeof(char));                                       
        memcpy(new_buffer + sizeof(char) * (buffer -> left + buffer -> size), buffer -> m_buffer + buffer -> left * sizeof(char), (buffer -> size - buffer -> left));                                                                     
                                                                                                                     
        buffer -> right += buffer -> size;                                                                           
        buffer -> size = new_size;                                                                                   
        free(buffer -> m_buffer);                                                                                    
        buffer -> m_buffer = new_buffer;                                                                             
}

void move_right(GapBuffer* buffer)                                                                                   
{                                                                                                                    
        if (buffer -> right < buffer -> size - 1)                                                                    
                move_gap_to(buffer, buffer -> right + 1);                                                            
}                                                                                                                    
                                                                                                                     
void move_left(GapBuffer* buffer) {                                                                                  
        if (buffer -> left > 0)                                                                                      
                move_gap_to(buffer, buffer -> left - 1);                                                             
}                                                                                                                    
                                                                                                                     
void add_char(GapBuffer* buffer , char c)                                                                            
{                                                                                                                    
        if (buffer -> left > buffer -> right)                                                                        
                expand_gap(buffer);                                                                                  
        buffer -> m_buffer[buffer -> left++] = c;                                                                    
}

void print_buffer(GapBuffer* b)                                                                                      
{                                                                                                                    
                                                                                                                     
        char out[b -> size*2 + 1];                                                                                   
        out[0] = '\r';                                                                                               
                                                                                                                     
        int c = 1;                                                                                                   
                                                                                                                     
        for (int i = 0; i < b -> size; i++)                                                                          
        {                                                                                                            
                if (i >= b -> left && i <= b -> right)                                                               
                {                                                                                                    
                        out[c++] = '_';                                                                              
                        out[c++] = ' ';                                                                              
                } else {                                                                                             
                        out[c++] = b -> m_buffer[i];                                                                 
                        out[c++] = ' ';                                                                              
                }                                                                                                    
        }                                                                                                            
                                                                                                                     
        write(STDIN_FILENO, out, b -> size * 2 + 4);                                                                 
                                                                                                                     
} 

void enable_raw_mode()                                                                                               
{                                                                                                                    
        struct termios t;                                                                                            
                                                                                                                     
        tcgetattr(STDIN_FILENO, &t);                                                                                 
                                                                                                                     
        t.c_iflag &= ~(BRKINT  | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);                                    
        t.c_oflag &= ~OPOST;                                                                                         
        t.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);                                                      
        t.c_cflag &= ~(CSIZE | PARENB);                                                                              
        t.c_cflag |= CS8;                                                                                            
                                                                                                                     
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &t);                                                                      
                                                                                                                     
} 

int main()                                                                                                           
{                                                                                                                    
                                                                                                                     
        enable_raw_mode();                                                                                           
                                                                                                                     
        GapBuffer* buffer = create_gap_buffer(16);                                                                   
                                                                                                                     
        while (true) {                                                                                               
                                                                                                                     
                fflush(stdout);                                                                                      
                char buff[4];                                                                                        
                read(STDIN_FILENO, buff, 4);                                                                         
                                                                                                                     
                if (buff[0] == '\x1b' && buff[1] == '[' && buff[2] == 'C')                                           
                        move_right(buffer);                                                                          
                else if (buff[0] == '\x1b' && buff[1] == '[' && buff[2] == 'D')                                      
                        move_left(buffer);                                                                           
                else if (buff[0] == '\b')                                                                            
                        delete_prev(buffer);                                                                         
                else if (buff[0] == '\x1b' && buff[1] == '[' && buff[2] == '3' && buff[3] == '~')                    
                        delete_next(buffer);                                                                         
                else if (buff[0] != '\n' && buff[0] != '\r')                                                         
                        add_char(buffer, buff[0]);                                                                   
                                                                                                                     
                print_buffer(buffer);                                                                                
                                                                                                                     
        } 

        destroy_gap_buffer(buffer);                                                                                  
        return 0;                                                                                                    
}
