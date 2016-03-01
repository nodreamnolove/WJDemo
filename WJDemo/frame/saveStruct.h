
#ifndef saveStruct_h
#define saveStruct_h

typedef struct _vehicleDimensionsType {
     byte vehicleLength[2];
     byte vehicleWidth;
     byte vehicleHeight;
}vehicleDimensionsType;

typedef struct _PROG_ICC0002_INFO {
    byte IccBanlance[4];
} PROG_ICC0002_INFO;


typedef struct _PROG_ICC0012_INFO {
    byte InRoadNetID[2];
    byte InRoadID[2];
    byte InRoadLandNO;
    byte InRoadTime[4];
    byte VehType;
    byte InOutStatus;
    byte Identificationstation[9];
    byte StaffID[3];
    byte InRoadShift;
    byte bindedPlate[12];
    byte OtherInfo[4];
} PROG_ICC0012_INFO;



typedef struct _PROG_ICC0015_INFO {
     byte cardIssuerID[8];
     byte cardType;
     byte cardVersion;
     byte roadswebID[2];
     byte cardNo[8];
     byte SignedDate[4];
     byte ExpiredDate[4];
     byte bindedPlate[12];
     byte userType;
     byte OtherInfo[2];
} PROG_ICC0015_INFO;

typedef struct _PKG_ICCINFO_DATA {
      PROG_ICC0002_INFO ICC0002_INFO;
      PROG_ICC0012_INFO ICC0012_INFO;
      PROG_ICC0015_INFO ICC0015_INFO;
} PKG_ICCINFO_DATA;

typedef struct _PKG_DATA {
     byte contractProvider[8];
     byte contractType;
     byte contractVersion;
     byte contractSerialNumber[8];
     byte contractSignedDate[4];
     byte contractExpiredDate[4];
     byte tamperFlag;
    
     byte vehicleLicencePlateNumber[12];
     byte vehicleLicencePlateColor[2];
     byte vehicleClass;
     byte vehicleUserType;
     vehicleDimensionsType vehicleDimensions;
     byte vehicleWheels;
     byte vehicleAxles;
     byte vehicleWheelBases[2];
     byte vehicleWeightLimits[3];
     byte vehicleSpecificInformation[16];
     byte vehicleEngineNumber[16];
     byte OBUID[4];
     byte C3Command[255];
     long C3Len;
} PKG_DATA;

#endif
