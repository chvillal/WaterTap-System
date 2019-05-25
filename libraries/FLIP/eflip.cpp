//
//  eflip.cpp
//  eFLIP
//
//  Created by Christopher Villalpando on 5/5/19.
//  Copyright © 2019 Christopher Villalpando. All rights reserved.
//

/* LIBRARIES */
#include "eflip.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <iostream>

/* DEFINES */
#define FLIP_CONT1      0x800000
#define FLIP_CONT2      0x008000
#define FLIP_CONT3      0x000080
#define GTP_CONT1       0x8000
#define GTP_CONT2       0x0080

#define MAX_SOCK_COUNT  2
/* CLASS FUNCTIONS */

void FlipSocket::clear_metafields()
{
    m_metafields.version = 0;
    m_metafields.dest_hst = 0;
    m_metafields.dest_net = 0;
    m_metafields.type = 0;
    m_metafields.ttl = 0;
    m_metafields.flow = 0;
    m_metafields.src_hst = 0;
    m_metafields.src_net = 0;
    m_metafields.length = 0;    //dynamic
    m_metafields.checksum = 0;  //dynamic
    m_metafields.offset = 0;
}

void FlipSocket::set_metabit(int32_t bitmask, bool state)
{
    switch(bitmask)
    {
        case FLIP_CONT1:
            m_metaheader.cont1 = state;
            break;
        case FLIP_CONT2:
            m_metaheader.cont2 = state;
            break;
        case FLIP_CONT3:
            m_metaheader.cont3 = state;
            break;
        case FLIP_ESP:
            m_metaheader.esp = state;
            break;
        case FLIP_VERSION:
            m_metaheader.ver = state;
            break;
        case FLIP_DEST_1:
            m_metaheader.dest2 = state;
            //m_metaheader.dest1 = false;
            break;
        case FLIP_DEST_4:
            m_metaheader.dest1 = state;
            //m_metaheader.dest2 = false;
            break;
        case FLIP_DEST_16:
            m_metaheader.dest1 = state;
            m_metaheader.dest2 = state;
            break;
        case FLIP_TYPE:
            m_metaheader.type = state;
            break;
        case FLIP_TTL:
            m_metaheader.ttl = state;
            break;
        case FLIP_FLOW:
            m_metaheader.flow = state;
            break;
        case FLIP_SOURCE_1:
            m_metaheader.source2 = state;
            //m_metaheader.source1 = false;
            break;
        case FLIP_SOURCE_4:
            m_metaheader.source1 = state;
            //m_metaheader.source2 = false;
            break;
        case FLIP_SOURCE_16:
            m_metaheader.source1 = state;
            m_metaheader.source2 = state;
            break;
        case FLIP_LENGTH:
            m_metaheader.length = state;
            break;
        case FLIP_CHECKSUM:
            m_metaheader.checksum = state;
            break;
        case FLIP_NOFRAG:
            m_metaheader.no_frag = state;
            break;
        case FLIP_OPT1:
            m_metaheader.opt1 = state;
            break;
        case FLIP_OPT2:
            m_metaheader.opt2 = state;
            break;
        case FLIP_FRAGOFFSET:
            m_metaheader.frag_offset = state;
            break;
        case FLIP_LASTFRAG:
            m_metaheader.last_frag = state;
            break;
        case FLIP_OPT3:
            m_metaheader.opt3 = state;
            break;
        case FLIP_OPT4:
            m_metaheader.opt4 = state;
            break;
        case FLIP_OPT5:
            m_metaheader.opt5 = state;
            break;
        case FLIP_OPT6:
            m_metaheader.opt6 = state;
            break;
        case FLIP_OPT7:
            m_metaheader.opt7 = state;
            break;
        default:
            break;
    }
}

bool FlipSocket::get_metabit(uint32_t bitmask)
{
    switch(bitmask)
    {
        case FLIP_ESP:
            return m_metaheader.esp;
        case FLIP_VERSION:
            return m_metaheader.ver;
        case FLIP_DEST_1:
            return m_metaheader.dest2;
        case FLIP_DEST_4:
            return m_metaheader.dest1;
        case FLIP_DEST_16:
            return (m_metaheader.dest1 && m_metaheader.dest2);
        case FLIP_TYPE:
            return m_metaheader.type;
        case FLIP_TTL:
            return m_metaheader.ttl;
        case FLIP_FLOW:
            return m_metaheader.flow;
        case FLIP_SOURCE_1:
            return m_metaheader.source2;
        case FLIP_SOURCE_4:
            return m_metaheader.source1;
        case FLIP_SOURCE_16:
            return (m_metaheader.source1 && m_metaheader.source2);
        case FLIP_LENGTH:
            return m_metaheader.length;
        case FLIP_CHECKSUM:
            return m_metaheader.checksum;
        case FLIP_NOFRAG:
            return m_metaheader.no_frag;
        case FLIP_OPT1:
            return m_metaheader.opt1;
        case FLIP_OPT2:
            return m_metaheader.opt2;
        case FLIP_FRAGOFFSET:
            return m_metaheader.frag_offset;
        case FLIP_LASTFRAG:
            return m_metaheader.last_frag;
        case FLIP_OPT3:
            return m_metaheader.opt3;
        case FLIP_OPT4:
            return m_metaheader.opt4;
        case FLIP_OPT5:
            return m_metaheader.opt5;
        case FLIP_OPT6:
            return m_metaheader.opt6;
        case FLIP_OPT7:
            return m_metaheader.opt7;
        case FLIP_CONT1:
            //std::cout << "\nLOG: get_metabit(): return bool: " << m_metaheader.cont1 << std::endl;
            return m_metaheader.cont1;
        case FLIP_CONT2:
            return m_metaheader.cont2;
        case FLIP_CONT3:
            return m_metaheader.cont3;
        default:
            break;
    }
    //if it gets here, error
    return false;
}

void FlipSocket::clear_metaheader()
{
    m_metaheader.esp = false;
    m_metaheader.ver = false;
    m_metaheader.dest1 = false;
    m_metaheader.dest2 = false;
    m_metaheader.type = false;
    m_metaheader.ttl = false;
    m_metaheader.flow = false;
    m_metaheader.source1 = false;
    m_metaheader.source2 = false;
    m_metaheader.length = false;
    m_metaheader.checksum = false;
    m_metaheader.no_frag = false;
    m_metaheader.opt1 = false;
    m_metaheader.opt2 = false;
    m_metaheader.frag_offset = false;
    m_metaheader.last_frag = false;
    m_metaheader.opt3 = false;
    m_metaheader.opt4 = false;
    m_metaheader.opt5 = false;
    m_metaheader.opt6 = false;
    m_metaheader.opt7 = false;
}

void FlipSocket::set_cont_bits()
{
    
    // if any bitfields set on 3rd byte, enable continuation bits 1 and 2
    if ( m_metaheader.frag_offset || m_metaheader.last_frag || m_metaheader.opt3 || m_metaheader.opt4 ||
         m_metaheader.opt5 || m_metaheader.opt6 || m_metaheader.opt7 )
    {
        m_metaheader.cont1 = true;
        m_metaheader.cont2 = true;
        
    } else if ( m_metaheader.source1 || m_metaheader.source2 || m_metaheader.length || m_metaheader.checksum ||
        m_metaheader.no_frag || m_metaheader.opt1 || m_metaheader.opt2 )
    {
        m_metaheader.cont1 = true;
        m_metaheader.cont2 = false;
        //std::cout << "\nLOG: set_cont_bits: cont1=true : " << m_metaheader.cont1 << std::endl;
    } else {
        m_metaheader.cont1 = false;
        m_metaheader.cont2 = false;
        //std::cout << "LOG: set_cont_bits: cont1=false\n" ;
    }
    
    //redundant under current version, but set for sanity.
    m_metaheader.cont3 = false;
}

void GTPsocket::set_metabit(uint16_t bitmask, bool state)
{
    switch (bitmask)
    {
        case GTP_CONT1:
            m_metaheader.cont1 = state;
            break;
        case GTP_FLAGS:
            m_metaheader.flags = state;
            break;
        case GTP_SOURCE:
            m_metaheader.src = state;
            break;
        case GTP_DEST:
            m_metaheader.dest = state;
            break;
        case GTP_SEQ:
            m_metaheader.seq = state;
            break;
        case GTP_ACK:
            m_metaheader.ack = state;
            break;
        case GTP_TIMESTAMP:
            m_metaheader.tstamp = state;
            break;
        case GTP_CHECKSUM:
            m_metaheader.checksum = state;
            break;
        case GTP_CONT2:
            m_metaheader.cont2 = state;
            break;
        case GTP_W_SIZE:
            m_metaheader.wsize = state;
            break;
        case GTP_URGENT:
            m_metaheader.urgent = state;
            break;
        case GTP_LENGTH:
            m_metaheader.len = state;
            break;
        case GTP_NEXTPROTO:
            m_metaheader.nextp = state;
            break;
        case GTP_OPT1:
            m_metaheader.opt1 = state;
            break;
        case GTP_OPT2:
            m_metaheader.opt2 = state;
            break;
        case GTP_OPT3:
            m_metaheader.opt3 = state;
            break;
        default:
            break;
    }
}

bool GTPsocket::get_metabit(uint16_t bitmask)
{
    switch (bitmask)
    {
        case GTP_CONT1:
            return m_metaheader.cont1;
        case GTP_FLAGS:
            return m_metaheader.flags;
        case GTP_SOURCE:
            return m_metaheader.src;
        case GTP_DEST:
            return m_metaheader.dest;
        case GTP_SEQ:
            return m_metaheader.seq;
        case GTP_ACK:
            return m_metaheader.ack;
        case GTP_TIMESTAMP:
            return m_metaheader.tstamp;
        case GTP_CHECKSUM:
            return m_metaheader.checksum;
        case GTP_CONT2:
            return m_metaheader.cont2;
        case GTP_W_SIZE:
            return m_metaheader.wsize;
        case GTP_URGENT:
            return m_metaheader.urgent;
        case GTP_LENGTH:
            return m_metaheader.len;
        case GTP_NEXTPROTO:
            return m_metaheader.nextp;
        case GTP_OPT1:
            return m_metaheader.opt1;
        case GTP_OPT2:
            return m_metaheader.opt2;
        case GTP_OPT3:
            return m_metaheader.opt3;
        default:
            break;
    }
    
    //if it gets here, error
    return false;
}

void GTPsocket::clear_metaheader()
{
    m_metaheader.flags = false;
    m_metaheader.src = false;
    m_metaheader.dest = false;
    m_metaheader.seq = false;
    m_metaheader.ack = false;
    m_metaheader.tstamp = false;
    m_metaheader.checksum = false;
    m_metaheader.wsize = false;
    m_metaheader.urgent = false;
    m_metaheader.len = false;
    m_metaheader.nextp = false;
    m_metaheader.opt1 = false;
    m_metaheader.opt2 = false;
    m_metaheader.opt3 = false;
}

void GTPsocket::clear_metafields()
{
    m_metafields.flags = 0;
    m_metafields.src = 0;
    m_metafields.dest = 0;
    m_metafields.seq = 0;
    m_metafields.ack = 0;
    m_metafields.tstamp = 0;
    m_metafields.checksum = 0;
    m_metafields.wsize = 0;
    m_metafields.urgent = 0;
    m_metafields.len = 0;
    m_metafields.nextp = 0;
}

void GTPsocket::set_cont_bits()
{
    if ( m_metaheader.wsize || m_metaheader.urgent || m_metaheader.len || m_metaheader.nextp ||
         m_metaheader.opt1 || m_metaheader.opt2 || m_metaheader.opt3 )
    {
        m_metaheader.cont1 = true;
    } else {
        m_metaheader.cont1 = false;
    }
    
    // additional metaheader bystes pending, limited to 2 bytes for now.
    m_metaheader.cont2 = false;
}

void SocketHandler::build_flip_metaheader(FlipSocket s)
{
    //int bitmap_size;
    //uint8_t m_bitmap[FLIP_MAX_BITMAP_SIZE+1]{};
    
    // update continuation bits now
    s.set_cont_bits();
    s.get_metabit(FLIP_CONT1)   ? m_bitmap[0] = m_bitmap[0] | (FLIP_CONT1 >> 16)  : m_bitmap[0] = m_bitmap[0] & (~FLIP_CONT1 >> 16);
    s.get_metabit(FLIP_CONT2)   ? m_bitmap[1] = m_bitmap[1] | (FLIP_CONT2 >> 8)   : m_bitmap[1] = m_bitmap[1] & (~FLIP_CONT2 >> 8);
    s.get_metabit(FLIP_CONT3)   ? m_bitmap[2] = m_bitmap[2] | (FLIP_CONT3)        : m_bitmap[2] = m_bitmap[2] & (~FLIP_CONT3);
    
    //parse flipsocket enabled fields to bitmap
    s.get_metabit(FLIP_ESP)     ? m_bitmap[0] = m_bitmap[0] | (FLIP_ESP >> 16)    : m_bitmap[0] = m_bitmap[0] & (~FLIP_ESP >> 16);
    s.get_metabit(FLIP_VERSION) ? m_bitmap[0] = m_bitmap[0] | (FLIP_VERSION>> 16) : m_bitmap[0] = m_bitmap[0] & (~FLIP_VERSION >> 16);
    s.get_metabit(FLIP_DEST_4)  ? m_bitmap[0] = m_bitmap[0] | (FLIP_DEST_4 >> 16) : m_bitmap[0] = m_bitmap[0] & (~FLIP_DEST_4 >> 16);
    s.get_metabit(FLIP_DEST_1)  ? m_bitmap[0] = m_bitmap[0] | (FLIP_DEST_1 >> 16) : m_bitmap[0] = m_bitmap[0] & (~FLIP_DEST_1 >> 16);
    s.get_metabit(FLIP_TYPE)    ? m_bitmap[0] = m_bitmap[0] | (FLIP_TYPE >> 16)   : m_bitmap[0] = m_bitmap[0] & (~FLIP_TYPE >> 16);
    s.get_metabit(FLIP_TTL)     ? m_bitmap[0] = m_bitmap[0] | (FLIP_TTL >> 16)    : m_bitmap[0] = m_bitmap[0] & (~FLIP_TTL >> 16);
    s.get_metabit(FLIP_FLOW)    ? m_bitmap[0] = m_bitmap[0] | (FLIP_FLOW >> 16)   : m_bitmap[0] = m_bitmap[0] & (~FLIP_FLOW >> 16);
    
    s.get_metabit(FLIP_SOURCE_4) ? m_bitmap[1] = m_bitmap[1] | (FLIP_SOURCE_4 >> 8) : m_bitmap[1] = m_bitmap[1] & (~FLIP_SOURCE_4 >> 8);
    s.get_metabit(FLIP_SOURCE_1) ? m_bitmap[1] = m_bitmap[1] | (FLIP_SOURCE_1 >> 8) : m_bitmap[1] = m_bitmap[1] & (~FLIP_SOURCE_1 >> 8);
    s.get_metabit(FLIP_LENGTH)   ? m_bitmap[1] = m_bitmap[1] | (FLIP_LENGTH >> 8)   : m_bitmap[1] = m_bitmap[1] & (~FLIP_LENGTH >> 8);
    s.get_metabit(FLIP_CHECKSUM) ? m_bitmap[1] = m_bitmap[1] | (FLIP_CHECKSUM >> 8) : m_bitmap[1] = m_bitmap[1] & (~FLIP_CHECKSUM >> 8);
    s.get_metabit(FLIP_NOFRAG)   ? m_bitmap[1] = m_bitmap[1] | (FLIP_NOFRAG >> 8)   : m_bitmap[1] = m_bitmap[1] & (~FLIP_NOFRAG >> 8);
    s.get_metabit(FLIP_OPT1)     ? m_bitmap[1] = m_bitmap[1] | (FLIP_OPT1 >> 8)     : m_bitmap[1] = m_bitmap[1] & (~FLIP_OPT1 >> 8);
    s.get_metabit(FLIP_OPT2)     ? m_bitmap[1] = m_bitmap[1] | (FLIP_OPT2 >> 8)     : m_bitmap[1] = m_bitmap[1] & (~FLIP_OPT2 >> 8);
    
    s.get_metabit(FLIP_FRAGOFFSET) ? m_bitmap[2] = m_bitmap[2] | FLIP_FRAGOFFSET : m_bitmap[2] = m_bitmap[2] & ~FLIP_FRAGOFFSET;
    s.get_metabit(FLIP_LASTFRAG)   ? m_bitmap[2] = m_bitmap[2] | FLIP_LASTFRAG   : m_bitmap[2] = m_bitmap[2] & ~FLIP_LASTFRAG;
    s.get_metabit(FLIP_OPT3)       ? m_bitmap[2] = m_bitmap[2] | FLIP_OPT3       : m_bitmap[2] = m_bitmap[2] & ~FLIP_OPT3;
    s.get_metabit(FLIP_OPT4)       ? m_bitmap[2] = m_bitmap[2] | FLIP_OPT4       : m_bitmap[2] = m_bitmap[2] & ~FLIP_OPT4;
    s.get_metabit(FLIP_OPT5)       ? m_bitmap[2] = m_bitmap[2] | FLIP_OPT5       : m_bitmap[2] = m_bitmap[2] & ~FLIP_OPT5;
    s.get_metabit(FLIP_OPT6)       ? m_bitmap[2] = m_bitmap[2] | FLIP_OPT6       : m_bitmap[2] = m_bitmap[2] & ~FLIP_OPT6;
    s.get_metabit(FLIP_OPT7)       ? m_bitmap[2] = m_bitmap[2] | FLIP_OPT7       : m_bitmap[2] = m_bitmap[2] & ~FLIP_OPT7;

    if (s.get_metabit(FLIP_CONT3)){
        f_bitmap_size = 4;
    } else if (s.get_metabit(FLIP_CONT2)) {
        f_bitmap_size = 3;
    } else if (s.get_metabit(FLIP_CONT1)){
        f_bitmap_size = 2;
    } else {
        f_bitmap_size = 1;
    }
    
}

void SocketHandler::build_flip_metafields(FlipSocket s)
{
    //int fields_size;
    //uint8_t m_fields[FLIP_MAX_FIELDS_SIZE+1]{};
    int index = 0;
    
    if (s.get_metabit(FLIP_VERSION)){
        m_fields[index] = s.get_version();
        index++;
    }
    
    if (s.get_metabit(FLIP_DEST_16))
    {
        //first byte tells size, 1-16 for now
        //implementation pending
        
    } else if (s.get_metabit(FLIP_DEST_4)) {
        m_fields[index] = (s.get_dest() & 0xFF000000) >> 24;
        index++;
        m_fields[index] = (s.get_dest() & 0xFF0000) >> 16;
        index++;
        m_fields[index] = (s.get_dest() & 0xFF00) >> 8;
        index++;
        m_fields[index] = (s.get_dest() & 0xFF);
        index++;
        
    } else if (s.get_metabit(FLIP_DEST_1)) {
        m_fields[index] = (s.get_dest() & 0xFF);
        index++;
    }
    
    if (s.get_metabit(FLIP_TYPE)) {
        m_fields[index] = s.get_type();
        index++;
    }
    
    if (s.get_metabit(FLIP_TTL)) {
        m_fields[index] = s.get_ttl();
        index++;
    }
    
    if (s.get_metabit(FLIP_FLOW)) {
        m_fields[index] = (s.get_flow() & 0xFF000000) >> 24;
        index++;
        m_fields[index] = (s.get_flow() & 0xFF0000) >> 16;
        index++;
        m_fields[index] = (s.get_flow() & 0xFF00) >> 8;
        index++;
        m_fields[index] = (s.get_flow() & 0xFF);
        index++;
    }
    
    if (s.get_metabit(FLIP_SOURCE_16))
    {
        //first byte tells size, 1-16 for now
        //implementation pending
        
    } else if (s.get_metabit(FLIP_SOURCE_4)) {
        m_fields[index] = (s.get_src() & 0xFF000000) >> 24;
        index++;
        m_fields[index] = (s.get_src() & 0xFF0000) >> 16;
        index++;
        m_fields[index] = (s.get_src() & 0xFF00) >> 8;
        index++;
        m_fields[index] = (s.get_src() & 0xFF);
        index++;
        
    } else if (s.get_metabit(FLIP_SOURCE_1)) {
        m_fields[index] = (s.get_src() & 0xFF);
        index++;
    }
    
    if (s.get_metabit(FLIP_LENGTH)) {
        m_fields[index] = (s.get_len() & 0xFF00) >> 8;
        index++;
        m_fields[index] = (s.get_len() & 0xFF);
        index++;
    }
    
    if (s.get_metabit(FLIP_CHECKSUM)) {
        m_fields[index] = (s.get_checksum() & 0xFF00) >> 8;
        index++;
        m_fields[index] = (s.get_checksum() & 0xFF);
        index++;
    }
    
    if (s.get_metabit(FLIP_FRAGOFFSET)) {
        m_fields[index] = (s.get_offset() & 0xFF00) >> 8;
        index++;
        m_fields[index] = (s.get_offset() & 0xFF);
        index++;
    }
    
    //at this point, index is pointing to a null bit alfter the last field
    //in m_fields.
    if (index != 0){
        f_fields_size = index - 1;
    }
}

int SocketHandler::parse_flip_metaheader(FlipSocket *s, uint8_t *message, int m_size)
{
    
    int i = 0;
    if (m_size < 1) {return 0;};
    
    // check if larger than 1 byte bitmap
    (message[i] & (FLIP_CONT1 >> 16))   ? s->set_metabit(FLIP_CONT1, true)  : s->set_metabit(FLIP_CONT1, false);
    (message[i] & (FLIP_ESP >> 16))     ? s->set_metabit(FLIP_ESP, true)    : s->set_metabit(FLIP_ESP, false);
    (message[i] & (FLIP_VERSION >> 16)) ? s->set_metabit(FLIP_VERSION, true): s->set_metabit(FLIP_VERSION, false);
    (message[i] & (FLIP_DEST_4 >> 16))  ? s->set_metabit(FLIP_DEST_4, true) : s->set_metabit(FLIP_DEST_4 , false);
    (message[i] & (FLIP_DEST_1 >> 16))  ? s->set_metabit(FLIP_DEST_1, true) : s->set_metabit(FLIP_DEST_1 , false);
    (message[i] & (FLIP_TYPE >> 16))    ? s->set_metabit(FLIP_TYPE, true)   : s->set_metabit(FLIP_TYPE , false);
    (message[i] & (FLIP_TTL >> 16))     ? s->set_metabit(FLIP_TTL, true)    : s->set_metabit(FLIP_TTL , false);
    (message[i] & (FLIP_FLOW >> 16))    ? s->set_metabit(FLIP_FLOW, true)   : s->set_metabit(FLIP_FLOW , false);
    i++;
    
//    std::cout << std::hex << std::bitset<8> (*(message+0)) << " " ;
//    std::cout << std::hex << std::bitset<8> (*(message+1)) << " \n" ;
//    std::cout << "cont1: " << (message[0] & (FLIP_CONT1 >> 16)) << " \n";
//
//    if(message[0] & (FLIP_CONT1 >> 16)){
//        std::cout << "true cout1\n";
//    } else {
//        std::cout << "false cout1\n";
//    }
    
    if (s->get_metabit(FLIP_CONT1) == false){
        //std::cout << "byte1 exit ";
        return i;
    }
    
    (message[i] & (FLIP_CONT2 >> 8))    ? s->set_metabit(FLIP_CONT2, true)   : s->set_metabit(FLIP_CONT2 , false);
    (message[i] & (FLIP_SOURCE_4 >> 8)) ? s->set_metabit(FLIP_SOURCE_4, true): s->set_metabit(FLIP_SOURCE_4 , false);
    (message[i] & (FLIP_SOURCE_1 >> 8)) ? s->set_metabit(FLIP_SOURCE_1, true): s->set_metabit(FLIP_SOURCE_1 , false);
    (message[i] & (FLIP_LENGTH >> 8))   ? s->set_metabit(FLIP_LENGTH, true)  : s->set_metabit(FLIP_LENGTH , false);
    (message[i] & (FLIP_CHECKSUM >> 8)) ? s->set_metabit(FLIP_CHECKSUM, true): s->set_metabit(FLIP_CHECKSUM , false);
    (message[i] & (FLIP_NOFRAG >> 8))   ? s->set_metabit(FLIP_NOFRAG, true)  : s->set_metabit(FLIP_NOFRAG , false);
    (message[i] & (FLIP_OPT1 >> 8))     ? s->set_metabit(FLIP_OPT1, true)    : s->set_metabit(FLIP_OPT1 , false);
    (message[i] & (FLIP_OPT2 >> 8))     ? s->set_metabit(FLIP_OPT2, true)    : s->set_metabit(FLIP_OPT2 , false);
    i++;
    
    if (s->get_metabit(FLIP_CONT2) == false){
        //std::cout << "byte2 exit ";
        return i;
    }
    
    (message[i] & FLIP_CONT3 )      ? s->set_metabit(FLIP_CONT3, true)     : s->set_metabit(FLIP_CONT3 , false);
    (message[i] & FLIP_FRAGOFFSET ) ? s->set_metabit(FLIP_FRAGOFFSET, true): s->set_metabit(FLIP_FRAGOFFSET , false);
    (message[i] & FLIP_LASTFRAG )   ? s->set_metabit(FLIP_LASTFRAG, true)  : s->set_metabit(FLIP_LASTFRAG , false);
    (message[i] & FLIP_OPT3 )       ? s->set_metabit(FLIP_OPT3, true)      : s->set_metabit(FLIP_OPT3 , false);
    (message[i] & FLIP_OPT4 )       ? s->set_metabit(FLIP_OPT4, true)      : s->set_metabit(FLIP_OPT4 , false);
    (message[i] & FLIP_OPT5 )       ? s->set_metabit(FLIP_OPT5, true)      : s->set_metabit(FLIP_OPT5 , false);
    (message[i] & FLIP_OPT6 )       ? s->set_metabit(FLIP_OPT6, true)      : s->set_metabit(FLIP_OPT6 , false);
    (message[i] & FLIP_OPT7 )       ? s->set_metabit(FLIP_OPT7, true)      : s->set_metabit(FLIP_OPT7 , false);
    i++;
    
    if (s->get_metabit(FLIP_CONT3) == false){
        //std::cout << "byte3 exit ";
        return i;
    }
    //if gets here -> error : not implemented yet
    return -1;
}

int SocketHandler::parse_flip_metafields(FlipSocket *s, uint8_t *message, int m_size, int index)
{
    int i = index;
    uint32_t byte4_t;
    
    s->clear_metafields();
    
    if (s->get_metabit(FLIP_VERSION)) {
        s->set_version(message[i]);
        i++;
    }
    
    if (s->get_metabit(FLIP_DEST_16)) {
        //implementation pending
        
    } else if (s->get_metabit(FLIP_DEST_4)) {
        byte4_t = 0;
        byte4_t = message[i] << 24;
        i++;
        byte4_t = byte4_t | (message[i] << 16);
        i++;
        byte4_t = byte4_t | (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        s->set_dest(byte4_t);

    } else if (s->get_metabit(FLIP_DEST_1)) {
        s->set_dest(message[i]);
        i++;
    }
    
    if (s->get_metabit(FLIP_TYPE)) {
        s->set_type(message[i]);
        i++;
    }
    
    if (s->get_metabit(FLIP_TTL)) {
        s->set_ttl(message[i]);
        i++;
    }
    
    if (s->get_metabit(FLIP_FLOW)) {
        byte4_t = 0;
        byte4_t = message[i] << 24;
        i++;
        byte4_t = byte4_t | (message[i] << 16);
        i++;
        byte4_t = byte4_t | (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        s->set_flow(byte4_t);
    }
    
    if (s->get_metabit(FLIP_SOURCE_16)) {
        //implementation pending
        
    } else if (s->get_metabit(FLIP_SOURCE_4)) {
        byte4_t = 0;
        byte4_t = message[i] << 24;
        i++;
        byte4_t = byte4_t | (message[i] << 16);
        i++;
        byte4_t = byte4_t | (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        s->set_src(byte4_t);
        
    } else if (s->get_metabit(FLIP_SOURCE_1)) {
        s->set_src(message[i]);
        i++;
    }
    
    if (s->get_metabit(FLIP_LENGTH)) {
        byte4_t = 0;
        byte4_t = (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        s->set_len(byte4_t);
    }
    
    if (s->get_metabit(FLIP_CHECKSUM)) {
        byte4_t = 0;
        byte4_t = (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        s->set_checksum(byte4_t);
    }
    
    if (s->get_metabit(FLIP_FRAGOFFSET)) {
        byte4_t = 0;
        byte4_t = (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        s->set_offset(byte4_t);
    }
    
    //return index pointing to buffer next byte (start of upper layer msg)
    if (index == i)
        return index;
    else
        return i - 1;
}

void SocketHandler::build_gtp_metaheader(GTPsocket g)
{
    //set cont bits first
    g.set_cont_bits();
    
    //insert continuation bits first
    g.get_metabit(GTP_CONT1) ? g_bitmap[0] = g_bitmap[0] | (GTP_CONT1 >> 8) : g_bitmap[0] = g_bitmap[0] & (~GTP_CONT1  >> 8);
    g.get_metabit(GTP_CONT2) ? g_bitmap[1] = g_bitmap[1] | (GTP_CONT2) : g_bitmap[1] = g_bitmap[1] & (~GTP_CONT2);
    
    //build byte0
    g.get_metabit(GTP_FLAGS) ? g_bitmap[0] = g_bitmap[0] | (GTP_FLAGS >> 8) : g_bitmap[0] = g_bitmap[0] & (~GTP_FLAGS  >> 8);
    g.get_metabit(GTP_SOURCE) ? g_bitmap[0] = g_bitmap[0] | (GTP_SOURCE >> 8) : g_bitmap[0] = g_bitmap[0] & (~GTP_SOURCE  >> 8);
    g.get_metabit(GTP_DEST) ? g_bitmap[0] = g_bitmap[0] | (GTP_DEST >> 8) : g_bitmap[0] = g_bitmap[0] & (~GTP_DEST  >> 8);
    g.get_metabit(GTP_SEQ) ? g_bitmap[0] = g_bitmap[0] | (GTP_SEQ >> 8) : g_bitmap[0] = g_bitmap[0] & (~GTP_SEQ  >> 8);
    g.get_metabit(GTP_ACK) ? g_bitmap[0] = g_bitmap[0] | (GTP_ACK >> 8) : g_bitmap[0] = g_bitmap[0] & (~GTP_ACK  >> 8);
    g.get_metabit(GTP_TIMESTAMP) ? g_bitmap[0] = g_bitmap[0] | (GTP_TIMESTAMP >> 8) : g_bitmap[0] = g_bitmap[0] & (~GTP_TIMESTAMP >> 8);
    g.get_metabit(GTP_CHECKSUM) ? g_bitmap[0] = g_bitmap[0] | (GTP_CHECKSUM >> 8) : g_bitmap[0] = g_bitmap[0] & (~GTP_CHECKSUM  >> 8);
    
    if (g.get_metabit(GTP_CONT1) == false)
    {
        g_bitmap_size = 1;
        return;
    }
    
    //build byte1
    g.get_metabit(GTP_W_SIZE) ? g_bitmap[1] = g_bitmap[1] | GTP_W_SIZE  : g_bitmap[1] = g_bitmap[1] & ~GTP_W_SIZE;
    g.get_metabit(GTP_URGENT) ? g_bitmap[1] = g_bitmap[1] | GTP_URGENT  : g_bitmap[1] = g_bitmap[1] & ~GTP_URGENT;
    g.get_metabit(GTP_LENGTH) ? g_bitmap[1] = g_bitmap[1] | GTP_LENGTH  : g_bitmap[1] = g_bitmap[1] & ~GTP_LENGTH;
    g.get_metabit(GTP_NEXTPROTO) ? g_bitmap[1] = g_bitmap[1] | GTP_NEXTPROTO  : g_bitmap[1] = g_bitmap[1] & ~GTP_NEXTPROTO;
    g.get_metabit(GTP_OPT1) ? g_bitmap[1] = g_bitmap[1] | GTP_OPT1  : g_bitmap[1] = g_bitmap[1] & ~GTP_OPT1;
    g.get_metabit(GTP_OPT2) ? g_bitmap[1] = g_bitmap[1] | GTP_OPT2  : g_bitmap[1] = g_bitmap[1] & ~GTP_OPT2;
    g.get_metabit(GTP_OPT3) ? g_bitmap[1] = g_bitmap[1] | GTP_OPT3  : g_bitmap[1] = g_bitmap[1] & ~GTP_OPT3;
    
    g_bitmap_size = 2;
    return;
}

void SocketHandler::build_gtp_metafields(GTPsocket g)
{
    int index = 0;
    
    if (g.get_metabit(GTP_FLAGS)){
        g_fields[index] = g.get_flags();
        index++;
    }
    
    if (g.get_metabit(GTP_SOURCE)){
        g_fields[index] = (g.get_src() & 0xFF00) >> 8;
        index++;
        g_fields[index] = (g.get_src() & 0xFF);
        index++;
    }
    
    if (g.get_metabit(GTP_DEST)){
        g_fields[index] = (g.get_dest() & 0xFF00) >> 8;
        index++;
        g_fields[index] = (g.get_dest() & 0xFF);
        index++;
    }
    
    if (g.get_metabit(GTP_SEQ)){
        g_fields[index] = (g.get_seq() & 0xFF000000) >> 24;
        index++;
        g_fields[index] = (g.get_seq() & 0xFF0000) >> 16;
        index++;
        g_fields[index] = (g.get_seq() & 0xFF00) >> 8;
        index++;
        g_fields[index] = (g.get_seq() & 0xFF);
        index++;
    }
    
    if (g.get_metabit(GTP_ACK)){
        g_fields[index] = (g.get_ack() & 0xFF000000) >> 24;
        index++;
        g_fields[index] = (g.get_ack() & 0xFF0000) >> 16;
        index++;
        g_fields[index] = (g.get_ack() & 0xFF00) >> 8;
        index++;
        g_fields[index] = (g.get_ack() & 0xFF);
        index++;
    }
    
    if (g.get_metabit(GTP_TIMESTAMP)){
        g_fields[index] = (g.get_tstamp() & 0xFF000000) >> 24;
        index++;
        g_fields[index] = (g.get_tstamp() & 0xFF0000) >> 16;
        index++;
        g_fields[index] = (g.get_tstamp() & 0xFF00) >> 8;
        index++;
        g_fields[index] = (g.get_tstamp() & 0xFF);
        index++;
    }
    
    if (g.get_metabit(GTP_CHECKSUM)){
        g_fields[index] = (g.get_checksum() & 0xFF00) >> 8;
        index++;
        g_fields[index] = (g.get_checksum() & 0xFF);
        index++;
    }

    if (g.get_metabit(GTP_W_SIZE)){
        g_fields[index] = (g.get_wsize() & 0xFF000000) >> 24;
        index++;
        g_fields[index] = (g.get_wsize() & 0xFF0000) >> 16;
        index++;
        g_fields[index] = (g.get_wsize() & 0xFF00) >> 8;
        index++;
        g_fields[index] = (g.get_wsize() & 0xFF);
        index++;
    }
    
    if (g.get_metabit(GTP_URGENT)){
        g_fields[index] = (g.get_urgent() & 0xFF00) >> 8;
        index++;
        g_fields[index] = (g.get_urgent() & 0xFF);
        index++;
    }
    
    if (g.get_metabit(GTP_LENGTH)){
        g_fields[index] = (g.get_len() & 0xFF00) >> 8;
        index++;
        g_fields[index] = (g.get_len() & 0xFF);
        index++;
    }
    
    if (g.get_metabit(GTP_NEXTPROTO)){
        g_fields[index] = g.get_nextp();
        index++;
    }
    
    if (index !=0)
        g_fields_size = index-1;
    
}

int SocketHandler::parse_gtp_metaheader(GTPsocket *g, uint8_t *message, int m_size)
{
    int i = 0;
    if (m_size < 1) {return 0;};
    
    // check if larger than 1 byte bitmap
    (message[i] & (GTP_CONT1 >> 8))   ? g->set_metabit(GTP_CONT1 , true)  : g->set_metabit(GTP_CONT1 , false);
    (message[i] & (GTP_FLAGS >> 8))   ? g->set_metabit(GTP_FLAGS , true)  : g->set_metabit(GTP_FLAGS , false);
    (message[i] & (GTP_SOURCE >> 8))   ? g->set_metabit(GTP_SOURCE , true)  : g->set_metabit(GTP_SOURCE , false);
    (message[i] & (GTP_DEST >> 8))   ? g->set_metabit(GTP_DEST , true)  : g->set_metabit(GTP_DEST , false);
    (message[i] & (GTP_SEQ >> 8))   ? g->set_metabit(GTP_SEQ , true)  : g->set_metabit(GTP_SEQ , false);
    (message[i] & (GTP_ACK >> 8))   ? g->set_metabit(GTP_ACK , true)  : g->set_metabit(GTP_ACK , false);
    (message[i] & (GTP_TIMESTAMP >> 8))   ? g->set_metabit(GTP_TIMESTAMP , true)  : g->set_metabit(GTP_TIMESTAMP , false);
    (message[i] & (GTP_CHECKSUM >> 8))   ? g->set_metabit(GTP_CHECKSUM , true)  : g->set_metabit(GTP_CHECKSUM , false);
    i++;
    
    if (g->get_metabit(GTP_CONT1) ==  false){
        return i;
    }
    
    (message[i] & GTP_CONT2)   ? g->set_metabit(GTP_CONT2 , true)  : g->set_metabit(GTP_CONT2 , false);
    (message[i] & GTP_W_SIZE)   ? g->set_metabit(GTP_W_SIZE , true)  : g->set_metabit(GTP_W_SIZE , false);
    (message[i] & GTP_URGENT)   ? g->set_metabit(GTP_URGENT , true)  : g->set_metabit(GTP_URGENT , false);
    (message[i] & GTP_LENGTH)   ? g->set_metabit(GTP_LENGTH , true)  : g->set_metabit(GTP_LENGTH , false);
    (message[i] & GTP_NEXTPROTO)   ? g->set_metabit(GTP_NEXTPROTO , true)  : g->set_metabit(GTP_NEXTPROTO , false);
    (message[i] & GTP_OPT1)   ? g->set_metabit(GTP_OPT1 , true)  : g->set_metabit(GTP_OPT1 , false);
    (message[i] & GTP_OPT2)   ? g->set_metabit(GTP_OPT2 , true)  : g->set_metabit(GTP_OPT2 , false);
    (message[i] & GTP_OPT3)   ? g->set_metabit(GTP_OPT3 , true)  : g->set_metabit(GTP_OPT3 , false);
    i++;
    
    return i;
}

int SocketHandler::parse_gtp_metafields(GTPsocket *g, uint8_t *message, int m_size, int index)
{
    int i = index;
    uint32_t byte4_t;
    
    g->clear_metafields();
    
    if (g->get_metabit(GTP_FLAGS)) {
        g->set_flags(message[i]);
        i++;
    }
    
    if (g->get_metabit(GTP_SOURCE)) {
        byte4_t = 0;
        byte4_t = (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        g->set_src(byte4_t);
    }
    
    if (g->get_metabit(GTP_DEST)) {
        byte4_t = 0;
        byte4_t = (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        g->set_dest(byte4_t);
    }
    
    if (g->get_metabit(GTP_SEQ)) {
        byte4_t = 0;
        byte4_t = message[i] << 24;
        i++;
        byte4_t = byte4_t | (message[i] << 16);
        i++;
        byte4_t = byte4_t | (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        g->set_seq(byte4_t);
    }
    
    if (g->get_metabit(GTP_ACK)) {
        byte4_t = 0;
        byte4_t = message[i] << 24;
        i++;
        byte4_t = byte4_t | (message[i] << 16);
        i++;
        byte4_t = byte4_t | (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        g->set_ack(byte4_t);
    }
    
    if (g->get_metabit(GTP_TIMESTAMP)) {
        byte4_t = 0;
        byte4_t = message[i] << 24;
        i++;
        byte4_t = byte4_t | (message[i] << 16);
        i++;
        byte4_t = byte4_t | (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        g->set_tstamp(byte4_t);
    }
    
    if (g->get_metabit(GTP_CHECKSUM)) {
        byte4_t = 0;
        byte4_t = (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        g->set_checksum(byte4_t);
    }
    
    if (g->get_metabit(GTP_W_SIZE)) {
        byte4_t = 0;
        byte4_t = message[i] << 24;
        i++;
        byte4_t = byte4_t | (message[i] << 16);
        i++;
        byte4_t = byte4_t | (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        g->set_wsize(byte4_t);
    }
    
    if (g->get_metabit(GTP_URGENT)) {
        byte4_t = 0;
        byte4_t = (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        g->set_urgent(byte4_t);
    }
    
    if (g->get_metabit(GTP_LENGTH)) {
        byte4_t = 0;
        byte4_t = (message[i] << 8);
        i++;
        byte4_t = byte4_t | message[i];
        i++;
        
        g->set_len(byte4_t);
    }
    
    if (g->get_metabit(GTP_NEXTPROTO)) {
        g->set_nextp(message[i]);
        i++;
    }
    
    if (index == i)
        return index;
    else
        return i - 1;
}


/* WRAPPER FUNCTIONS */
int FlipKernel::socket()
{
    //check if sockets left in memory, else return error
    if (s_index >= MAX_SOCK_COUNT) {
        return -1;
    }
    
    s_index++;
    
    //return index to sockethandler array element
    return (s_index - 1);
}

int FlipKernel::setsocketopt(int s, uint8_t sock_type, uint32_t option, uint32_t value)
{
    if (sock_type == SOCK_TYPE_FLIP) {
        switch (option)
        {
            case FLIP_VERSION :
                sockets[s].flip_s.set_metabit(option, true);
                sockets[s].flip_s.set_version(value);
                break;
            case FLIP_DEST_1 :
                sockets[s].flip_s.set_metabit(option, true);
                sockets[s].flip_s.set_dest(value);
                break;
            case FLIP_DEST_4 :
                sockets[s].flip_s.set_metabit(option, true);
                sockets[s].flip_s.set_dest(value);
                break;
            case FLIP_DEST_16 :
                sockets[s].flip_s.set_metabit(option, true);
                //sockets[s].flip_s.set_dest(value)
                break;
            case FLIP_TYPE :
                sockets[s].flip_s.set_metabit(option, true);
                sockets[s].flip_s.set_type(value);
                break;
            case FLIP_TTL :
                sockets[s].flip_s.set_metabit(option, true);
                sockets[s].flip_s.set_ttl(value);
                break;
            case FLIP_FLOW :
                sockets[s].flip_s.set_metabit(option, true);
                sockets[s].flip_s.set_flow(value);
                break;
            case FLIP_SOURCE_1 :
                sockets[s].flip_s.set_metabit(option, true);
                sockets[s].flip_s.set_src(value);
                break;
            case FLIP_SOURCE_4 :
                sockets[s].flip_s.set_metabit(option, true);
                sockets[s].flip_s.set_src(value);
                break;
            case FLIP_SOURCE_16 :
                sockets[s].flip_s.set_metabit(option, true);
                //sockets[s].flip_s.set_src(value);
                break;
            case FLIP_LENGTH :
                sockets[s].flip_s.set_metabit(option, true);
                sockets[s].flip_s.set_len(value);
                break;
            case FLIP_CHECKSUM :
                sockets[s].flip_s.set_metabit(option, true);
                sockets[s].flip_s.set_checksum(value);
                break;
            case FLIP_NOFRAG :
                sockets[s].flip_s.set_metabit(option, true);
                break;
            case FLIP_FRAGOFFSET :
                //sockets[s].flip_s.set_metabit(option, true);
                break;
            case FLIP_LASTFRAG :
                //sockets[s].flip_s.set_metabit(option, true);
                break;
            case FLIP_OPT1 :
                sockets[s].flip_s.set_metabit(option, true);
                //sockets[s].flip_s.set_opt1(value);
                break;
            case FLIP_OPT2 :
                sockets[s].flip_s.set_metabit(option, true);
                //sockets[s].flip_s.set_opt2(value);
                break;
            case FLIP_OPT3 :
                sockets[s].flip_s.set_metabit(option, true);
                //sockets[s].flip_s.set_opt3(value);
                break;
            case FLIP_OPT4 :
                sockets[s].flip_s.set_metabit(option, true);
                //sockets[s].flip_s.set_opt4(value);
                break;
            case FLIP_OPT5 :
                sockets[s].flip_s.set_metabit(option, true);
                //sockets[s].flip_s.set_opt5(value);
                break;
            case FLIP_OPT6 :
                sockets[s].flip_s.set_metabit(option, true);
                //sockets[s].flip_s.set_opt6(value);
                break;
            case FLIP_OPT7 :
                sockets[s].flip_s.set_metabit(option, true);
                //sockets[s].flip_s.set_opt7(value);
                break;
            default:
                //wrong option - return error
                return -1;
                break;
        }
        return 0;
        
    } else if (sock_type == SOCK_TYPE_GTP) {
        switch (option)
        {
            case GTP_FLAGS :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_flags(value);
                break;
            case GTP_SOURCE :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_src(value);
                break;
            case GTP_DEST :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_dest(value);
                break;
            case GTP_SEQ :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_seq(value);
                break;
            case GTP_ACK :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_ack(value);
                break;
            case GTP_TIMESTAMP :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_tstamp(value);
                break;
            case GTP_CHECKSUM :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_checksum(value);
                break;
            case GTP_W_SIZE :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_wsize(value);
                break;
            case GTP_URGENT :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_urgent(value);
                break;
            case GTP_LENGTH :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_len(value);
                break;
            case GTP_NEXTPROTO :
                sockets[s].gtp_s.set_metabit(option, true);
                sockets[s].gtp_s.set_nextp(value);
                break;
            case GTP_OPT1 :
                sockets[s].gtp_s.set_metabit(option, true);
                //sockets[s].gtp_s.set_opt1();;
                break;
            case GTP_OPT2 :
                sockets[s].gtp_s.set_metabit(option, true);
                //sockets[s].gtp_s.set_opt2();
                break;
            case GTP_OPT3 :
                sockets[s].gtp_s.set_metabit(option, true);
                //sockets[s].gtp_s.set_opt3();
                break;
            default:
                //wrong option - return error
                return -1;
                break;
        }
        return 0;
        
    }
    
    //if gets here, error - invalid socket type
    return -1;
}

//get socket options
uint32_t FlipKernel::getsocketopt(int s, uint8_t sock_type, uint32_t option)
{
    if (sock_type == SOCK_TYPE_FLIP){
        switch (option)
        {
            case FLIP_VERSION:
                return sockets[0].flip_s.get_version();
            case FLIP_DEST_1:
                return (uint32_t) sockets[0].flip_s.get_dest();
            case FLIP_DEST_4:
                return (uint32_t) sockets[0].flip_s.get_dest();
            case FLIP_DEST_16:
                //return sockets[0].flip_s.get_dest();
                break;
            case FLIP_TYPE:
                return sockets[0].flip_s.get_type();
            case FLIP_TTL:
                return sockets[0].flip_s.get_ttl();
            case FLIP_FLOW:
                return sockets[0].flip_s.get_flow();
            case FLIP_SOURCE_1:
                return (uint32_t) sockets[0].flip_s.get_src();
            case FLIP_SOURCE_4:
                return (uint32_t) sockets[0].flip_s.get_src();
            case FLIP_SOURCE_16:
                //return sockets[0].flip_s.get_src();
                break;
            case FLIP_LENGTH:
                return sockets[0].flip_s.get_len();
            case FLIP_CHECKSUM:
                return sockets[0].flip_s.get_checksum();
//            case FLIP_NOFRAG:
//                break;
//            case FLIP_FRAGOFFSET:
//                break;
//            case FLIP_LASTFRAG:
//                break;
            case FLIP_OPT1:
                break;
            case FLIP_OPT2:
                break;
            case FLIP_OPT3:
                break;
            case FLIP_OPT4:
                break;
            case FLIP_OPT5:
                break;
            case FLIP_OPT6:
                break;
            case FLIP_OPT7:
                break;
            default:
                //error - invalid option
                return -1;
                break;
        }
        return 0;
        
    } else if (sock_type == SOCK_TYPE_GTP){
        switch (option)
        {
            case GTP_FLAGS :
                return sockets[s].gtp_s.get_flags();
            case GTP_SOURCE :
                return sockets[s].gtp_s.get_src();
            case GTP_DEST :
                return sockets[s].gtp_s.get_dest();
            case GTP_SEQ :
                return sockets[s].gtp_s.get_seq();
            case GTP_ACK :
                return sockets[s].gtp_s.get_ack();
            case GTP_TIMESTAMP :
                return sockets[s].gtp_s.get_tstamp();
            case GTP_CHECKSUM :
                return sockets[s].gtp_s.get_checksum();
            case GTP_W_SIZE :
                return sockets[s].gtp_s.get_wsize();
            case GTP_URGENT :
                return sockets[s].gtp_s.get_urgent();
            case GTP_LENGTH :
                return sockets[s].gtp_s.get_len();
            case GTP_NEXTPROTO :
                return sockets[s].gtp_s.get_nextp();
            case GTP_OPT1 :
                break;
            case GTP_OPT2 :
                break;
            case GTP_OPT3 :
                break;
            default:
                //error - invalid option
                return -1;
                break;
        }
        return 0;
        
    }
    
    //if it gets here, return error - invalid socket type
    return -1;
}

//write new message
int FlipKernel::write(int s, char *buf, int len)
{
    int g_size;
    int f_size;
    uint8_t *g_bitmap;
    uint8_t *f_bitmap;
    int index = 0;
    
    //set continuation bits for flip and gtp metaheaders
    sockets[s].gtp_s.set_cont_bits();
    sockets[s].flip_s.set_cont_bits();

    //build flip and gtp metaheaders
    sockets[s].build_gtp_metaheader(sockets[s].gtp_s);
    sockets[s].build_flip_metaheader(sockets[s].flip_s);
    
    //get pointers to flip and gtp str buffers
    f_bitmap = sockets[s].get_flip_bitmap();
    g_bitmap = sockets[s].get_gtp_bitmap();
    
    //get size of flip and gtp str's
    f_size = sockets[s].get_flip_bitmapsize();
    g_size = sockets[s].get_gtp_bitmapsize();
    
    //copy flip headers to pacekt
    if (f_bitmap[0] > 0) {
        for (index = 0 ; index < f_size; index++){
            sockets[s].snt_buffer[index] = f_bitmap[index];
        }
        
        sockets[s].build_flip_metafields(sockets[s].flip_s);
        f_bitmap = sockets[s].get_flip_metafields();
        f_size = sockets[s].get_flip_fieldssize();
        
        for (int i = 0; i <= f_size; i++){
            sockets[s].snt_buffer[index] = f_bitmap[i];
            index++;
        }
    }
    
    //copy gtp headers to packet
    if ( g_bitmap[0] > 0) {
        for (int i = 0; i < g_size; i++){
            sockets[s].snt_buffer[index] = g_bitmap[i];
            index++;
        }
        
        sockets[s].build_gtp_metafields(sockets[s].gtp_s);
        g_bitmap = sockets[s].get_gtp_metafields();
        g_size = sockets[s].get_gtp_fieldsize();
        
        for (int i = 0; i <= g_size; i++){
            sockets[s].snt_buffer[index] = g_bitmap[i];
            index++;
        }
    }
    
    //copy messg string from caller app.
    for (int i=0; i < len; i++){
        sockets[s].snt_buffer[index] = (uint8_t) buf[i];
        index++;
    }
    
    //flag kernel there is new data to send
    toSend++;
    //add message to ptr
    ptr[s] = sockets[s].snt_buffer;
    msglen[s] = index - 1;
    
    return 0;
}

//wait and reat new message?
int FlipKernel::read(int s, char *buf, int len)
{
    return 0;
}

void FlipKernel::kernel()
{
//    if (read_from_phy(buf, &buflen)){
//        //received packet - analyze it!
//
//    }
    
    if (toSend > 0){
        //send next msg to lora
        for (int i = 0; i < KERNEL_QUEUE_SIZE; i++ ){
            if (ptr[i] != NULL){
//                    //print - remove after testing
//                    std::cout << "Packet in binary:\n";
//                    for (int j=0; j < msglen[i]; j++ ){
//                        std::cout << std::bitset<8> (*ptr[i] + j) << " " ;
//                    }
//                    std::cout << std::endl;
//                    std::cout << std::endl;
//
//                    std::cout << "Packet in *char:\n";
//                    for (int j=0; j<msglen[i]; j++ ){
//                        std::cout << (char) *(ptr[i] + j) << " " ;
//                    }
//                    std::cout << std::endl;
//                    std::cout << std::endl;
                
                write_to_phy( ptr[i], msglen[i]);
                ptr[i] = NULL;
                msglen[i] = 0;
                toSend--;
                break;
            }
        }
        
    }
    
}


void FlipKernel::init(bool (*write)(uint8_t*, uint8_t), bool (*read)(uint8_t*, uint8_t))
{
    write_to_phy = write;
    read_from_phy = read;
}

/* TEST/PRINT FUNCTIONS */
void print_metaheader(FlipSocket s)
{
    std::cout << "cont1: " << s.get_metabit(FLIP_CONT1) ;
    std::cout << "\nesp: " << s.get_metabit(FLIP_ESP);
    std::cout << "\nver: " << s.get_metabit(FLIP_VERSION);
    std::cout << "\ndest1: " << s.get_metabit(FLIP_DEST_4);
    std::cout << "\ndest2: " << s.get_metabit(FLIP_DEST_1);
    std::cout << "\ntype: " << s.get_metabit(FLIP_TYPE);
    std::cout << "\nttl: " << s.get_metabit(FLIP_TTL);
    std::cout << "\nflow: " << s.get_metabit(FLIP_FLOW);
    std::cout << "\ncont2: " << s.get_metabit(FLIP_CONT2);
    std::cout << "\nsource1: " << s.get_metabit(FLIP_SOURCE_4);
    std::cout << "\nsource2: " << s.get_metabit(FLIP_SOURCE_1);
    std::cout << "\nlength: " << s.get_metabit(FLIP_LENGTH);
    std::cout << "\nchecksum: " << s.get_metabit(FLIP_CHECKSUM);
    std::cout << "\nno_frag: " << s.get_metabit(FLIP_NOFRAG);
    std::cout << "\nopt1: " << s.get_metabit(FLIP_OPT1);
    std::cout << "\nopt2: " << s.get_metabit(FLIP_OPT2);
    std::cout << "\ncont3: " << s.get_metabit(FLIP_CONT3);
    std::cout << "\nfrag_offset: " << s.get_metabit(FLIP_FRAGOFFSET);
    std::cout << "\nlast-frag: " << s.get_metabit(FLIP_LASTFRAG);
    std::cout << "\nopt3: " << s.get_metabit(FLIP_OPT3);
    std::cout << "\nopt4: " << s.get_metabit(FLIP_OPT4);
    std::cout << "\nopt5: " << s.get_metabit(FLIP_OPT5);
    std::cout << "\nopt6: " << s.get_metabit(FLIP_OPT6);
    std::cout << "\nopt7: " << s.get_metabit(FLIP_OPT7);
    std::cout << "\n";
}

void print_metafields(FlipSocket s)
{
    std::cout << std::dec << "Ver: " <<   (int) s.get_version();
    std::cout << std::dec << "\nDest:" << (int) s.get_dest();
    std::cout << std::dec << "\nType:" << (int) s.get_type();
    std::cout << std::dec << "\nTTL:" <<  (int) s.get_ttl();
    std::cout << std::dec << "\nFlow:" << (int) s.get_flow();
    std::cout << std::dec << "\nSrc:" <<  (int) s.get_src();
    std::cout << std::dec << "\nLen:" <<  (int) s.get_len();
    std::cout << std::dec << "\nCRC:" <<  (int) s.get_checksum();
    std::cout << std::dec << "\nOffset:" << (int) s.get_offset();
    std::cout << "\n";
}

void print_gtp_metaheader(GTPsocket g)
{
    std::cout << "\ncont1: " << g.get_metabit(GTP_CONT1);
    std::cout << "\nflags: " << g.get_metabit(GTP_FLAGS);
    std::cout << "\nsrc: " << g.get_metabit(GTP_SOURCE);
    std::cout << "\ndest: " << g.get_metabit(GTP_DEST);
    std::cout << "\nseq#: " << g.get_metabit(GTP_SEQ);
    std::cout << "\nack: " << g.get_metabit(GTP_ACK);
    std::cout << "\ntstmp: " << g.get_metabit(GTP_TIMESTAMP);
    std::cout << "\nchksum: " << g.get_metabit(GTP_CHECKSUM);
    std::cout << "\ncont2: " << g.get_metabit(GTP_CONT2);
    std::cout << "\nwsize: " << g.get_metabit(GTP_W_SIZE);
    std::cout << "\nurgent: " << g.get_metabit(GTP_URGENT);
    std::cout << "\nlen: " << g.get_metabit(GTP_LENGTH);
    std::cout << "\nnextp: " << g.get_metabit(GTP_NEXTPROTO);
}

void print_gtp_metafields(GTPsocket g)
{
    std::cout << std::dec << "\nflags: " <<   (int) g.get_flags() ;
    std::cout << std::dec << "\nsrc: " <<   (int) g.get_src() ;
    std::cout << std::dec << "\ndest: " <<   (int) g.get_dest() ;
    std::cout << std::dec << "\nseq: " <<   (int) g.get_seq() ;
    std::cout << std::dec << "\nack: " <<   (int) g.get_ack() ;
    std::cout << std::dec << "\ntstamp: " <<   (int) g.get_tstamp() ;
    std::cout << std::dec << "\nchecksum: " <<   (int) g.get_checksum() ;
    std::cout << std::dec << "\nwsize: " <<   (int) g.get_wsize() ;
    std::cout << std::dec << "\nurgent: " <<   (int) g.get_urgent() ;
    std::cout << std::dec << "\nlen: " <<   (int) g.get_len() ;
    std::cout << std::dec << "\nnextp: " <<   (int) g.get_nextp() ;
}
