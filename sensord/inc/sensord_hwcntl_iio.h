/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Robert Bosch GmbH. All rights reserved. 
 * Copyright (C) 2011~2015 Bosch Sensortec GmbH All Rights Reserved
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SENSORD_HWCNTL_IIO_H_
#define SENSORD_HWCNTL_IIO_H_

/**
 *
 * @param name
 * @param iio_dir
 * @return
 */
static inline int get_IIOnum_by_name(const char *name, const char *iio_dir)
{
#define IIO_NAME_MAXLEN 30
#define MAX_FILENAME_LEN 256
    const char *type = "iio:device";
    struct dirent *ent;
    struct dirent dirent;
    int number, numstrlen;

    FILE *nameFile;
    DIR *dp;
    char thisname[IIO_NAME_MAXLEN];
    char fname_buf[MAX_FILENAME_LEN+1];
    int ret;

    dp = opendir(iio_dir);
    if (NULL == dp)
    {
        return -ENODEV;
    }

    while (!readdir_r(dp, &dirent, &ent) && NULL != ent)
    {
        if (0 == strcmp(ent->d_name, ".") ||
                0 == strcmp(ent->d_name, "..") ||
                strlen(ent->d_name) <= strlen(type) ||
                0 != strncmp(ent->d_name, type, strlen(type)))
        {
            /*filter impossible dir names*/
            continue;
        }

        numstrlen = sscanf(ent->d_name + strlen(type), "%d", &number);

        /* verify the next character is not a colon */
        if(0 == strncmp(ent->d_name + strlen(type) + numstrlen, ":", 1))
        {
            continue;
        }

        snprintf(fname_buf, MAX_FILENAME_LEN, "%s%s%d/name", iio_dir, type, number);

        nameFile = fopen(fname_buf, "r");
        if (!nameFile)
        {
            continue;
        }

        ret = fscanf(nameFile, "%s", thisname);
        if(ret <= 0)
        {
            fclose(nameFile);
            break;
        }

        if (0 == strcmp(name, thisname))
        {
            fclose(nameFile);
            closedir(dp);
            return number;
        }

        fclose(nameFile);
    }

    closedir(dp);
    return -ENODEV;
}

#define MAX_FILENAME_LEN 256

static inline int wr_sysfs_twoint(const char *filename, char *basedir, int val1, int val2)
{
    FILE *fp;
    char fname_buf[MAX_FILENAME_LEN+1];

    snprintf(fname_buf, MAX_FILENAME_LEN, "%s/%s", basedir, filename);

    fp = fopen(fname_buf, "w");
    if (NULL == fp)
    {
        return -errno;
    }

    fprintf(fp, "%d %d", val1, val2);
    fclose(fp);

    return 0;
}

static inline  int wr_sysfs_oneint(const char *filename, char *basedir, int val)
{
    FILE *fp;
    char fname_buf[MAX_FILENAME_LEN+1];

    snprintf(fname_buf, MAX_FILENAME_LEN, "%s/%s", basedir, filename);

    fp = fopen(fname_buf, "w");
    if (NULL == fp)
    {
        return -errno;
    }

    fprintf(fp, "%d", val);
    fclose(fp);

    return 0;
}


static inline  int wr_sysfs_str(const char *filename, char *basedir, const char *str)
{
    FILE *fp;
    char fname_buf[MAX_FILENAME_LEN+1];

    snprintf(fname_buf, MAX_FILENAME_LEN, "%s/%s", basedir, filename);

    fp = fopen(fname_buf, "w");
    if (NULL == fp)
    {
        return -errno;
    }

    fprintf(fp, "%s", str);
    fclose(fp);

    return 0;
}


static inline int rd_sysfs_oneint(const char *filename, char *basedir, int *pval)
{
    FILE *fp;
    char fname_buf[MAX_FILENAME_LEN+1];
    int ret;

    snprintf(fname_buf, MAX_FILENAME_LEN, "%s/%s", basedir, filename);

    fp = fopen(fname_buf, "r");
    if (NULL == fp)
    {
        return -errno;
    }

    ret = fscanf(fp, "%d\n", pval);
    fclose(fp);

    if(ret <= 0){
        return -errno;
    }

    return 0;
}


#endif /* SENSORD_HWCNTL_IIO_H_ */
