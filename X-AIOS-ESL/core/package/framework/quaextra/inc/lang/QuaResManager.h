/*
*
* Copyright (c) Quaming Intelligent Technology Co., Ltd.
* (C) Copyright All Rights Reserved.
*
*/

#ifndef __QUARESMANAGER_H__
#define __QUARESMANAGER_H__

#include "QuaResTypes.h"

QBool ResPkgManagerSetCurrentLang(int langId);
const char *ResPkgManagerGetLangSelfName(int landId);
int ResPkgManagerGetImeLangId(int langId);
int ResPkgManagerGetCurrentLang(void);
const char * GetTextRes(ResID id);

#endif /* __QUARESMANAGER_H__ */
