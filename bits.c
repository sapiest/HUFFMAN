#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bits.h"

t_stream *fopen_bit(FILE *file, size_t size) {
    t_stream *tmp = (t_stream *) malloc(sizeof(t_stream));
    tmp->fid = file;
    tmp->ind = 0;
    tmp->len = size;
    //memset(tmp->buff, '\0',BUFF_SIZE);
    tmp->offset = BITS;
    return tmp;
}

void fflush_bit(t_stream *stream) {
    int add = 0;
    if (stream->offset != BITS)
        ++add;
    fwrite(stream->buff, 1, stream->ind + add, stream->fid);
    fflush(stream->fid);
    stream->offset = BITS;
    stream->ind = 0;
}

void fclose_bit(t_stream *stream){
    fflush_bit(stream);
    //memset(stream->buff, '\0',BUFF_SIZE);
    stream->ind = 0;
    stream->offset = 0;
    stream->len = 0;
    fclose(stream->fid);
}

int put_bit(BYTE bit, t_stream *stream) {
    bit = bit != 0;
    //printf("%i", (int) bit);
    if (bit != 0 && bit != 1)
        return EOF;
    if (bit == 1)
        stream->buff[stream->ind] |= 1 << stream->offset;
    else
        stream->buff[stream->ind] &= ~(1 << stream->offset);
    --(stream->offset);
    if (stream->offset == -1) {
        stream->offset = BITS;
        ++(stream->ind);
    }
    if (stream->ind == stream->len)
        fflush_bit(stream);
}

int fread_bit(t_stream *stream) {
    if (stream->ind == stream->len) {
        stream->len = fread(stream->buff, 1, BUFF_SIZE, stream->fid);
        stream->ind = 0;
    }
    if (stream->len == 0)
        return EOF;
    int bit;
    bit = (stream->buff[stream->ind] & (1 << stream->offset)) != 0;
    --(stream->offset);
    if (stream->offset == -1) {
        stream->offset = BITS;
        ++(stream->ind);
    }
    return bit;
}

int bit_feof(t_stream *stream){
    if(feof(stream->fid) && (stream->ind == stream->len))
        return EOF;
}