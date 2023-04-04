/**
 *============================================================================
 *  SYSTEM_NAME                 �古�u�`�m
 *  SUB-SYSTEM_NAME             ����
 *  PROGRAM_NAME                EBCDIC->ASCII�ϊ�
 *  PROGRAM_SOURCE              PE2A.c
 *  PROGRAM_WRITTEN             �L��@��㏤�H��c��
 *============================================================================
 */

#include       <stdio.h>
#include       <string.h>
#include       <stdlib.h>
#include       <time.h>

#define        TRUE            1       /* �^ */
#define        FALSE           0       /* �U */
#define        TMP_SIZE        80      /* ��ƈ�T�C�Y */

       FILE            *fin;
       FILE            *fout;
       int             err;

unsigned  char  ebc_asc_tbl[256] = {   /* EBCDIC-ASCII�ϊ��e�[�u�� */
        0x00 , 0x01 , 0x02 , 0x03 , 0x9C , 0x09 , 0x86 , 0x7F ,
        0x97 , 0x8D , 0x8E , 0x0B , 0x0C , 0x0D , 0x0E , 0x0F ,
        0x10 , 0x11 , 0x12 , 0x13 , 0x9D , 0x0A , 0x08 , 0x87 ,
        0x18 , 0x19 , 0x92 , 0x8F , 0x1C , 0x1D , 0x1E , 0x1F ,
        0x80 , 0x81 , 0x82 , 0x83 , 0x84 , 0x85 , 0x17 , 0x1B ,
        0x88 , 0x89 , 0x8A , 0x8B , 0x8C , 0x05 , 0x06 , 0x07 ,
        0x90 , 0x91 , 0x16 , 0x93 , 0x94 , 0x95 , 0x96 , 0x04 ,
        0x98 , 0x99 , 0x9A , 0x9B , 0x14 , 0x15 , 0x9E , 0x1A ,
        0x20 , 0xA1 , 0xA2 , 0xA3 , 0xA4 , 0xA5 , 0xA6 , 0xA7 ,
        0xA8 , 0xA9 , 0x5B , 0x2E , 0x3C , 0x28 , 0x2B , 0x21 ,
        0x26 , 0xAA , 0xAB , 0xAC , 0xAD , 0xAE , 0xAF , 0x61 ,
        0xB0 , 0x62 , 0x5D , 0x5C , 0x2A , 0x29 , 0x3B , 0x5E ,
        0x2D , 0x2F , 0x63 , 0x64 , 0x65 , 0x66 , 0x67 , 0x68 ,
        0x69 , 0x6A , 0x7C , 0x2C , 0x25 , 0x5F , 0x3E , 0x3F ,
        0x6B , 0x6C , 0x6D , 0x6E , 0x6F , 0x70 , 0x71 , 0x72 ,
        0x73 , 0x60 , 0x3A , 0x23 , 0x40 , 0x27 , 0x3D , 0x22 ,
        0x74 , 0xB1 , 0xB2 , 0xB3 , 0xB4 , 0xB5 , 0xB6 , 0xB7 ,
        0xB8 , 0xB9 , 0xBA , 0x75 , 0xBB , 0xBC , 0xBD , 0xBE ,
        0xBF , 0xC0 , 0xC1 , 0xC2 , 0xC3 , 0xC4 , 0xC5 , 0xC6 ,
        0xC7 , 0xC8 , 0xC9 , 0x76 , 0x77 , 0xCA , 0xCB , 0xCC ,
        0x78 , 0x7E , 0xCD , 0xCE , 0xCF , 0xD0 , 0xD1 , 0xD2 ,
        0xD3 , 0xD4 , 0xD5 , 0x79 , 0xD6 , 0xD7 , 0xD8 , 0xD9 ,
        0x7A , 0xA0 , 0xE0 , 0xE1 , 0xE2 , 0xE3 , 0xE4 , 0xE5 ,
        0xE6 , 0xE7 , 0xDA , 0xDB , 0xDC , 0xDD , 0xDE , 0xDF ,
        0x7B , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 ,
        0x48 , 0x49 , 0xE8 , 0xE9 , 0xEA , 0xEB , 0xEC , 0xED ,
        0x7D , 0x4A , 0x4B , 0x4C , 0x4D , 0x4E , 0x4F , 0x50 ,
        0x51 , 0x52 , 0xEE , 0xEF , 0xF0 , 0xF1 , 0xF2 , 0xF3 ,
        0x24 , 0x9F , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 , 0x58 ,
        0x59 , 0x5A , 0xF4 , 0xF5 , 0xF6 , 0xF7 , 0xF8 , 0xF9 ,
        0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 ,
        0x38 , 0x39 , 0xFA , 0xFB , 0xFC , 0xFD , 0xFE , 0xFF
};

/*-----------------------------------------------------------------------------
 *     �֐���`
 *-----------------------------------------------------------------------------
*/
int    outWriteCheck(void);

/*-----------------------------------------------------------------------------
 *     �o�̓t�@�C�� �������݌���
 *-----------------------------------------------------------------------------
 */
int    outWriteCheck(void)
{
   err = ferror(fout);
   if (err) {
       printf("�o�̓G���[ %2d:%s\r\n", err, strerror(err));
       return 1;
   }
   return 0;
}

/*-----------------------------------------------------------------------------
 *     �又��
 *-----------------------------------------------------------------------------
 */
int main(int argc, char *argv[])
{
       unsigned int    ix;             /* �Y�� */
       unsigned int    _len;           /* ���R�[�h�� */
       unsigned char   _cc;            /* ���̂P�o�C�g */
       char            tmpBuffer[TMP_SIZE];
       time_t          ctime;           /* current time. */

/* �J�n */
   printf("EBCDIC->ASCII�ϊ�\r\n");
   ctime = time(NULL);
   strftime (tmpBuffer, TMP_SIZE, "%Y/%m/%d(%a) %H:%M:%S", localtime(&ctime));
   printf("START %s\r\n", tmpBuffer);

/* �������� */
   switch(argc){ 
       case 4:
           break; 
       default:
           printf("PE2A.exe input output length\r\n");
           return 1;
   }

/* �t�@�C�����̐ݒ� */
   printf("���̓t�@�C���F%s\r\n", argv[1]);
   printf("�o�̓t�@�C���F%s\r\n", argv[2]);

/* ���R�[�h���̐ݒ� */
   _len = atoi(argv[3]);
   if (_len > 10000) {
       printf("�w�肳�ꂽ���R�[�h��������Ă��܂�\r\n");
       return 1;
   }
   printf("���R�[�h���F%d\r\n", _len);

	/* ���̓t�@�C���̃I�[�v�� */
   fin = fopen(argv[1], "rb");
   if (fin == NULL) {
       printf("���̓t�@�C���@�I�[�v���G���[�F%s\r\n", argv[1]);
       return 1;
   }

/* �o�̓t�@�C���̃I�[�v�� */
   fout = fopen(argv[2], "wb");
   if (fout == NULL) {
       printf("�o�̓t�@�C���@�I�[�v���G���[�F%s\r\n", argv[2]);
       return 1;
   }

   ix = 1;
   do {
       _cc = getc(fin);
       if (feof(fin))     break;
       err = ferror(fin);
       if (err) {
           printf("���̓G���[ %2d:%s\r\n", err, strerror(err));
           return 1;
       }
       _cc = ebc_asc_tbl[_cc];
       fprintf (fout, "%c", _cc);
       if (outWriteCheck()) return 1;

       if (_len > 0) {
           if (ix == _len) {     /* �ŏI���Ȃ���s�R�[�h�t�� */
               fprintf (fout, "\r\n");
               if (outWriteCheck()) return 1;
               ix = 1;
           } else {
               ix++;
           }
       }

   } while (TRUE);

/* �t�@�C���̃N���[�Y */
   fclose(fout);
   fclose(fin);

/* �I�� */
   printf("����I��\r\n");
   return 0;
}
