/****************************************************************************/
/* Copyright (c) 2017 MBARI                                                 */
/* MBARI Proprietary Information. All rights reserved.                      */
/****************************************************************************/
/* Summary  : Linux ed. Replay TRN using logged data from previous mission  */
/* Filename : Replay.h                                                      */
/* Author   : Henthorn                                                      */
/* Project  : Iceberg AUV                                                   */
/* Version  : 1.0                                                           */
/* Created  : 07/03/2017                                                    */
/* Modified :                                                               */
/* Archived :                                                               */
/****************************************************************************/
/* Modification History:                                                    */
/* Began with a copy of test_client as there is a lot of stuff to reuse     */
/****************************************************************************/
#ifndef _REPLAY_H_
#define _REPLAY_H_

#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>

class DataLogReader;
struct TRN_attr;
struct poseT;
struct measT;
class TerrainNav;

#define LCM_HOST  "USING.LCM.COMMS"   // Host id to indicate LCM usae
#define REPLAY_PATHNAME_LENGTH  300    // buffer lengths for full pathnames

#undef WITH_REPLAY_DEGTORAD

enum DvlCsvFields
{
    DVL_TIME = 0,
    DVL_NORTH,
    DVL_EAST,
    DVL_DEPTH,
    DVL_PSI,
    DVL_THETA,
    DVL_PHI,
    DVL_WX,
    DVL_WY,
    DVL_WZ,
    DVL_VX,
    DVL_VY,
    DVL_VZ,
    DVL_VALID,
    DVL_LOCK,
    DVL_NBEAMS,
    DVL_RANGES
};

struct TRN_attr
{
    char *_mapFileName;
    long  _map_type;
    long  _filter_type;
    char *_particlesName;
    char *_vehicleCfgName;
    char *_dvlCfgName;
    char *_resonCfgName;
    char *_terrainNavServer;
    char *_lrauvDvlFilename;
    long  _terrainNavPort;
    bool _forceLowGradeFilter;
    bool _allowFilterReinits;
    long _useModifiedWeighting;
    long _samplePeriod;
    double _maxNorthingCov;
    double _maxEastingCov;
    double _maxNorthingError;
    double _maxEastingError;
    double _phiBias;
    bool _useIDTData;
    bool _useDvlSide;
    bool _useMbTrnData;
};

class Replay {

public:
    // Seconds within which a DVL record matches TRN record
    static const double DVL4TRN;
    // Seconds within which a nav record matches TRN record
    static const double NAV4TRN;
    // array depth for VNORM
    static const uint16_t REPLAY_VNORM_DIM;

    Replay(const char* logdir, const char *map, const char *host=0, int port=0);
    ~Replay();
    int loadCfgAttributes();
    int openLogFiles();
    int getNextRecordSet(poseT *pt, measT *mt);
    int getNextTRNRecordSet(poseT *pt, measT *mt);

    bool useTRNServer();
    bool useLcmTrn();
    TerrainNav* connectTRN();

protected:
    char *logdir;
    double lastTime;
    TRN_attr *trn_attr;
    long nupdates, nreinits;

    // Use file streams instead of DataLogReaders
    DataLogReader *trn_log, *dvl_log, *nav_log, *mbtrn_log, *tnav_log;
    FILE *dvl_csv;

    int getNextKeyValue(FILE *cfg, char key[], char value[]);

    // MbTrn.log version of getNextRecordSet()
    int getMbTrnRecordSet(poseT *pt, measT *mt);

    // LRAUV CSV file version of getNextRecordSet()
    int getLRAUVDvlRecordSet(poseT *pt, measT *mt);
    int parseDvlCsvLine(const char *line, poseT *pt, measT *mt);
};

#endif
