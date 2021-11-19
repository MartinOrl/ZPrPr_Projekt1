#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 256

void getSegmentError(int segmentIndex, int rowIndex, char resultString[128]){
    switch (segmentIndex)
    {
    case 1:
        printf("Found error in row %d. Expected name format of \"NAME SURNAME\", instead saw %s\n",rowIndex, resultString);
        break;
    case 2:
        printf("Found error in row %d. Expected gender format of f or m, instead saw %s\n",rowIndex, resultString);
        break;
    case 3:
        printf("Found error in row %d. Expected year of birth format of YYYY, instead saw %s\n",rowIndex, resultString);
        break;
    case 4:
        printf("Found error in row %d. Expected brand name from array [porsche, bugatti, honda, ferrari], instead saw %s\n",rowIndex, resultString);
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        printf("Found error in row %d. Expected 5 rounds in format r1;r2;r3;r4;r5 . Instead saw \"%s\"\n",rowIndex, resultString);
        break;
    default:
        break;
    }
}

int checkFileIntegrity(){
    FILE *fr;
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");

    char row[MAXCHAR];
    char *rowSegment;
    int segmentIndex; // reference to look where we are in the row
    int rowIndex = 0; //reference to look which row we are in
    char helper[128]; // helper variable to help us with checking segments
    int temporaryIndex;
    int segmentLength = 0;
    int temporaryStatus = 0; // similar to integrityStatus, instead looks for error in the segment
    int integrityStatus = 1; // if 1, then integrity is valid, if 0 then data is in a corrupted format
    int segmentCount = 0; // variable to check if all 9 segments are present
    int roundsCount = 0; // variable to check if all 5 rounds are present

    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        rowIndex ++;
        if(feof(fr) != 0 && rowIndex == 1 && strlen(row) == 0){
            printf("\nData file is empty! Exiting function!\n");
            return 0;
        }
        rowSegment = strtok(row,";");
        segmentIndex = 0;
        segmentCount = 0;
        roundsCount = 0;
        while(rowSegment != NULL){
            segmentIndex++;
            segmentCount++;
            strcpy(helper, rowSegment);
            temporaryStatus = 0;
            if(segmentIndex == 1){
                segmentLength = strlen(helper);
                
                for(temporaryIndex = 0; temporaryIndex < segmentLength; temporaryIndex++){ 
                    if((int)helper[temporaryIndex] == 32 && (temporaryIndex+1) != segmentLength && (((int)helper[temporaryIndex + 1] > 64 && (int)helper[temporaryIndex + 1] < 91) || ((int)helper[temporaryIndex] > 96 && (int)helper[temporaryIndex] < 122)))  temporaryStatus++;
                }
                if((int)helper[segmentLength-1] == 32){
                    integrityStatus = 0;
                    getSegmentError(segmentIndex, rowIndex, "\' \'");
                }
                if(!temporaryStatus){
                    integrityStatus = 0;
                    getSegmentError(segmentIndex, rowIndex, helper);
                }
            }
            if(segmentIndex == 2){
                if(strcmp(helper, "m") && strcmp(helper, "f")){
                    integrityStatus = 0;
                    getSegmentError(segmentIndex, rowIndex, helper);
                }
            }
            if(segmentIndex == 3){
                if(!(int)atoi(helper) || (int)atoi(helper) < 1000){
                    integrityStatus = 0;
                    getSegmentError(segmentIndex, rowIndex, helper);
                }
            }
            if(segmentIndex == 4){
                if(strcmp(helper,"porsche") && strcmp(helper, "bugatti") && strcmp(helper, "honda") && strcmp(helper, "ferrari")){
                    integrityStatus = 0;
                    getSegmentError(segmentIndex, rowIndex, helper);
                }
            }
            if(segmentIndex >= 5 && segmentIndex < 10){
                if((float)atof(helper) > 0) roundsCount++;

                for(temporaryIndex = 0; temporaryIndex < strlen(helper); temporaryIndex++){
                    if((int)helper[temporaryIndex] == 32){
                        integrityStatus = 0;
                        getSegmentError(segmentIndex, rowIndex, helper);
                    }
                }
            }
            rowSegment = strtok(NULL, ";");
        }

        if(segmentCount == 1){
            printf("Found error in row %d. Empty line detected!\n", rowIndex);
            integrityStatus = 0;
            continue;
        }
        if(roundsCount != 5){
            integrityStatus = 0;
            sprintf(helper, "%d rounds.", roundsCount);
            getSegmentError(segmentIndex, rowIndex, helper);
        }
    }
    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
    return integrityStatus;
}

void sum(){
    FILE *fr;
    
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");
    char row[MAXCHAR];
    char *segment;
    char gender[2];
    int segmentHelperIndex;

    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        segment = strtok(row, ";");
        segmentHelperIndex = 0;
        while(segment != NULL){
            segmentHelperIndex++;
            if(segmentHelperIndex == 1){
                printf("%s, ",segment);
            }
            if(segmentHelperIndex == 2) strcpy(gender,segment);
            if(segmentHelperIndex == 3){
                printf("nar. %d, %s,", (int)atoi(segment), !strcmp(gender,"m") ? "muz" : "zena" );
            }
            if(segmentHelperIndex == 4){
                printf(" Automobil: %s\nCasy okruhov: ", segment);
            }
            if(segmentHelperIndex > 4 && segmentHelperIndex < 0){
                printf("%.3f;", (float)atof(segment));
            }
            if(segmentHelperIndex == 9){
                printf("%.3f\n",(float)atof(segment));
            }
            segment = strtok(NULL, ";");
        }
    }
    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
}

void rmdriver(){
    
    FILE *fr, *fh;;
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");
    if((fh = fopen("helper.csv","w")) == NULL) printf("subor sa neotvoril\n");

    char row[MAXCHAR];
    char *segment, *last;
    char name[64];
    int segmentHelperIndex = 0;
    int globalFound = 0;
    int found = 0;
    int added = 0;
    
    printf("Zadaj priezvisko jazdca: ");
    if(!scanf(" %s", name)){
        printf("Priezvisko nebolo zadané správne. Ukončujem funkciu!");
        return;
    }
    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        segment = strtok(row, ";");
        segmentHelperIndex = 0;
        while(segment != NULL){
            segmentHelperIndex++;
            if(segmentHelperIndex == 1 && !found){
                last = strrchr(segment, 32);
                if(strstr(last+1,name)){
                    found = 1;
                    globalFound = 1;
                    break;
                }
            }
            if(!found){
                if(segmentHelperIndex == 1){
                    if(added) fprintf(fh,"\n");
                    fprintf(fh,"%s;",segment);
                }
                if(segmentHelperIndex < 9 && segmentHelperIndex > 1) fprintf(fh,"%s;",segment);
                else if(segmentHelperIndex == 9) fprintf(fh,"%.3f",(float)atof(segment));
                added++;
            }
            segment = strtok(NULL, ";");
        }
        found = found ? 0 : found;
    }
    if(!globalFound) printf("No driver found with name %s", name);
   
    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
    if(fclose(fh) == EOF) printf("Subor sa nepodarilo zatvorit\n");
    
    if((fr = fopen("tabulka.csv", "w")) == NULL) printf("subor sa neotvoril\n");
    if((fh = fopen("helper.csv","r")) == NULL) printf("subor sa neotvoril\n");
    
    while(feof(fh) == 0){
        strcpy(row,"");
        fgets(row, MAXCHAR, fh);
        if(feof(fh) != 0 && strlen(row) == 0){
            return;
        }
        fprintf(fr,"%s",row);
    }
    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
    if(fclose(fh) == EOF) printf("Subor sa nepodarilo zatvorit\n");
}

void brand(){
    FILE *fr;
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");

    char row[MAXCHAR];
    char *segment;
    char name[64];
    char bestBrandNames[4][64];
    float bestRounds[5];
    int bestRoundsIndexes[4];
    char brands[4][8] = {"ferrari","bugatti","porsche","honda"};
    int segmentHelperIndex = 0;
    float currentRound = 0;
    int brandIndex = 0;

    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        segment = strtok(row, ";");
        segmentHelperIndex = 0;
        while(segment != NULL){
            segmentHelperIndex++;
            if(segmentHelperIndex == 1) strcpy(name, segment);
            
            if(segmentHelperIndex == 4){
                if(!strcmp(segment, "ferrari")) brandIndex = 0;
                if(!strcmp(segment, "bugatti")) brandIndex = 1;
                if(!strcmp(segment, "porsche")) brandIndex = 2;
                if(!strcmp(segment, "honda")) brandIndex = 3;
            }
            if(segmentHelperIndex > 4){
                currentRound = (float)atof(segment);
                if(bestRounds[brandIndex] == 0){
                    bestRounds[brandIndex] = currentRound;
                    strcpy(bestBrandNames[brandIndex], name);
                    bestRoundsIndexes[brandIndex] = (segmentHelperIndex - 4);
    
                }
                if(currentRound < bestRounds[brandIndex]){
                    bestRounds[brandIndex] = currentRound;
                    bestRoundsIndexes[brandIndex] = (segmentHelperIndex - 4);
                    strcpy(bestBrandNames[brandIndex], name);
                }
                
            }
            segment = strtok(NULL, ";");
        }
    }
    for(int i = 0; i < 4; i++){
        if(i > 0) printf("\n\n");
        printf("Znacka: %s\n", brands[i]);
        printf("Najlepsie kolo: %.3f\n", bestRounds[i]);
        printf("Jazdec: %s\n", bestBrandNames[i]);
        printf("Cislo kola: %d", bestRoundsIndexes[i]);
    }

    if(fclose(fr) == EOF)printf("Subor sa nepodarilo zatvorit\n");
}

void lap(){
    FILE *fr;
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");

    char row[MAXCHAR];
    char *segment;
    char name[64];
    int bestRoundNum;
    char bestName[64];
    float bestRound = 0;
    int segmentHelperIndex = 0;
    float currentRound = 0;

    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        segment = strtok(row, ";");
        segmentHelperIndex = 0;
        while(segment != NULL){
            segmentHelperIndex++;
            if(!bestRound && segmentHelperIndex == 5) bestRound = (float)atof(segment);
            
            if(segmentHelperIndex == 1) strcpy(name, segment);
            
            if(segmentHelperIndex > 4 && segmentHelperIndex < 10){
                currentRound = (float)atof(segment);
                if(currentRound < bestRound){
                    bestRound = currentRound; //sets best round to current number
                    bestRoundNum = segmentHelperIndex - 4;
                    strcpy(bestName, name);
                }
            }
            segment = strtok(NULL, ";");
        }
    }
    printf("Najlepsie kolo: %.3f\nJazdec: %s\nCislo kola: %d\n", bestRound, bestName, bestRoundNum);
  
    if(fclose(fr) == EOF)printf("Subor sa nepodarilo zatvorit\n");
}

void driver(){
    FILE *fr;
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");

    char row[MAXCHAR];
    char *segment, *last;
    char name[64];
    int year;
    int gender = 0;
    char brand[7];
    float bestRound, worstRound, roundSum;
    int segmentHelperIndex = 0;
    int found = 0;

    printf("Zadaj priezvisko jazdca: ");
    scanf(" %s", name);

    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        segment = strtok(row, ";");
        segmentHelperIndex = 0;
        while(segment != NULL){
            segmentHelperIndex++;
            if(segmentHelperIndex == 1){
                last = strrchr(segment, 32);
                if(strstr(last+1,name)){ 
                    found = 1;
                }
            }
            if(found){
                if(segmentHelperIndex == 1) printf("\n%s\n", segment);
                if(segmentHelperIndex == 2) gender = strcmp(segment, "f") ? 1 : 0;
                if(segmentHelperIndex == 3) year = (int)atoi(segment);
                if(segmentHelperIndex == 4){ 
                    strcpy(brand, segment);
                    printf("nar. %d, %s\nAutomobil: %s\nCasy okruhov: ",year, gender ? "muz" : "zena", brand);
                }
                if(segmentHelperIndex > 4){
                    printf("%.3f",(float)atof(segment));
                    if(segmentHelperIndex == 5){
                        bestRound = (float)atof(segment);
                        worstRound = (float)atof(segment);
                        roundSum += (float)atof(segment);
                    }
                    if(segmentHelperIndex > 5){
                        bestRound = (float)atof(segment) < bestRound ? (float)atof(segment) : bestRound;
                        worstRound = (float)atof(segment) > worstRound ? (float)atof(segment) : worstRound;
                        roundSum += (float)atof(segment);
                    }
                    if(segmentHelperIndex < 9) printf(";");
                    if(segmentHelperIndex == 9){
                        printf("\nNajlepsie kolo: %.3f\nNajhorsie kolo: %.3f\nPriemerne kolo: %.3f", bestRound, worstRound, roundSum / 5.0);
                        break;
                    }
                }
            }
            segment = strtok(NULL, ";");
        }
        if(found) break;
    }
    if(!found) printf("No driver found with name %s", name);

    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
}

void under(){
    FILE *fr;
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");
    char row[MAXCHAR];
    char *segment;
    float roundLimit;
    char name[64];
    int roundIndexes[5];
    int tempRoundIndex;
    float underRounds[5];
    int segmentHelperIndex = 0;
    float currentRound = 0.0;
    printf("Zadaj cas kola vo formate CC.CCC : ");

    if(!scanf(" %.3f", &roundLimit) && roundLimit <= 0.0){
        printf("Zadany neplatny cas, alebo zly format! Ukoncujem funkciu!");
        return;
    }
    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        segment = strtok(row, ";");
        segmentHelperIndex = 0;
        tempRoundIndex = 0;
        underRounds[0] = 0;
        while(segment != NULL){
            segmentHelperIndex++;
            if(segmentHelperIndex == 1) strcpy(name,segment);
            if(segmentHelperIndex > 4){
                currentRound = (float)atof(segment);
                if(currentRound < roundLimit){
                    underRounds[tempRoundIndex] = currentRound;
                    roundIndexes[tempRoundIndex] = segmentHelperIndex - 4;
                    tempRoundIndex++;
                }
            }
            segment = strtok(NULL, ";");
        }
        if(underRounds[0]){
            printf("%s - %d %s,",name,tempRoundIndex, tempRoundIndex != 1 && tempRoundIndex != 5 ? "kola" : "kol");
            for(int i = 1; i <= tempRoundIndex; i++){
                printf(" %d (%.3f)",roundIndexes[i-1],underRounds[i-1]);
            }
            printf("\n");
        }
    }
    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
}

void average(){
    FILE *fr;
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");

    char row[MAXCHAR];
    char *segment;
    char name[64];
    char bestName[64];
    float bestRound = 0;
    int segmentHelperIndex = 0;
    float averageRound = 0;

    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        segment = strtok(row, ";");
        segmentHelperIndex = 0;
        while(segment != NULL){
            segmentHelperIndex++;
            if(segmentHelperIndex == 1) strcpy(name, segment);
            if(segmentHelperIndex > 4) averageRound += ((float)atof(segment) / 5.0);
            segment = strtok(NULL, ";");

        }
        if(!bestRound){
            bestRound = averageRound;
            strcpy(bestName, name);
        }
        if(averageRound < bestRound){
            strcpy(bestName, name);
            bestRound = averageRound;
        }
        printf("%s - %.3f\n",name, averageRound);
        averageRound = 0.0;
    }
    printf("\nNajlepsie:\n%s - %.3f", bestName, bestRound);

    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
}

void gender(){
    FILE *fr;
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");
    char row[MAXCHAR];
    char *segment;
    char name[64];
    int bestRoundNum;
    char bestName[64];
    float bestRound = 0;
    int segmentHelperIndex = 0;
    float currentRound = 0;
    char gender;
    char driverGender;
    printf("Zadaj pohlavie (m - muz; f - zena): ");

    if(!scanf(" %c", &gender)){
        printf("Zadane zle pohlavie. Ukoncujem funkciu!");
        return;
    }
    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        segment = strtok(row, ";");
        segmentHelperIndex = 0;
    
        while(segment != NULL){
            segmentHelperIndex++;
            if(segmentHelperIndex == 1) strcpy(name, segment);
            
            if(segmentHelperIndex == 2) driverGender = !strcmp(segment, "m") ? 'm' : 'f';
            
            if(segmentHelperIndex == 5 && !bestRound) bestRound = (float)atof(segment);
            
            if(driverGender == gender){

                if(segmentHelperIndex > 4 && segmentHelperIndex < 10){
                    currentRound = (float)atof(segment);
                    if(currentRound < bestRound){
                        bestRound = currentRound; //sets best round to current one
                        bestRoundNum = segmentHelperIndex - 4;
                        strcpy(bestName, name);
                    }
                }
            }
            segment = strtok(NULL, ";");
        }
    }
    
    printf("Najlepsie kolo: %.3f\nJazdec: %s\nCislo kola: %d\n", bestRound, bestName, bestRoundNum);

    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
}

void newdriver(){
    
    FILE *fw;

    if((fw = fopen("tabulka.csv", "a")) == NULL) printf("subor sa neotvoril\n");


    char helper[64];
    char name[64];
    char brand[64];
    int year;
    int gender;
    char *roundsToken;
    int index = 0;
    float rounds[5];
    char temp[16];
    printf("Enter Driver Name: ");
    scanf(" %[^\n]", name);
    printf("Enter year of birth: ");
    scanf("%d", &year);
    printf("Enter gender (0 - male, 1 - female): ");
    scanf("%d", &gender);
    printf("Enter Car Brand (ferrari, porsche, bugatti, honda): ");
    scanf("%s", brand);
    printf("Enter rounds (format: round1;round2;round3): ");
    scanf("%s", helper);

    if(strcmp("bugatti",brand) && strcmp("ferrari",brand) && strcmp("honda",brand) && strcmp("porsche",brand)){
        printf("No valid brand! Driver addition failed! Exiting function");
        return;
    }

    int semicolonCount = 0;
    // Checking for rounds count
    for(int i = 0; i < 64; i++){
        if(helper[i] == 44){ 
            printf("Found a typo in the rounds. You wrote , instead of ;! Don't worry, I fixed %s\'s rounds for you.\n", name);
            helper[i] = ';';
        }
        if(helper[i] == 59) semicolonCount += 1;
    }

    if(semicolonCount == 4){
        roundsToken = strtok(helper,";");
        while(roundsToken != NULL){
            strcpy(temp, roundsToken);
            rounds[index] = (float)atof(temp);
            roundsToken = strtok(NULL, ";");
            index++;
        }
        fprintf(fw, "\n%s;%c;%d;%s;%.3f;%.3f;%.3f;%.3f;%.3f",name, !gender ? 'm' : 'f', year, brand, rounds[0],rounds[1],rounds[2],rounds[3],rounds[4]);
    } else {
        printf("Not Enough rounds written. Exiting function!");
        return;
    }
    

    if(fclose(fw) == EOF) printf("Subor sa nepodarilo zatvorit\n");
    if(checkFileIntegrity()){ 
        printf("\n");
        sum();
    }
    else return;
}

void change(){
    FILE *fr, *fh;;
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");
    if((fh = fopen("helper.csv","w")) == NULL) printf("subor sa neotvoril\n");
    
    char row[MAXCHAR];
    char *segment, *last;
    int targetRound;
    float newRound;
    char name[64];
    int segmentHelperIndex = 0;
    int globalFound = 0;
    int found = 0;
    int added = 0;
    
    printf("Zadaj priezvisko jazdca: ");
    if(!scanf(" %s", name)){
        printf("Priezvisko nebolo zadane spravne. Ukoncujem funkciu!"); 
        return;
    }
    printf("Zadaj cislo kola: ");
    if(!scanf("%d", &targetRound) || targetRound > 5 || targetRound < 1){
        printf("Bolo zadane zle cislo kola, alebo zly format. Ukoncujem funkciu!"); 
        return;
    }
    printf("Zadaj novy cas vo formate CC.CCC : ");
    if(!scanf("%f", &newRound) || newRound <= 0){
        printf("Bol zadany neplatny novy cas, alebo zly format. Ukoncujem funkciu!"); 
        return;
    }
    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        segment = strtok(row, ";");
        segmentHelperIndex = 0;
        while(segment != NULL){
            segmentHelperIndex++;
            if(segmentHelperIndex == 1 && !found){
                last = strrchr(segment, 32);
                if(strstr(last+1,name)){
                    found = 1;
                    globalFound = 1;
                }
            }
            if(found){
                
                if(segmentHelperIndex == 1 && added){
                    fprintf(fh,"\n");
                }
                if(segmentHelperIndex < 5){
                    fprintf(fh,"%s",segment);
                }
                if(segmentHelperIndex > 4){
                    fprintf(fh,"%.3f", (segmentHelperIndex-4) == targetRound ? newRound : (float)atof(segment));
                }
                if(segmentHelperIndex < 9) fprintf(fh,";");
                added++;
            }else{
                if(segmentHelperIndex == 1 && added){
                    fprintf(fh,"\n");
                }
                if(segmentHelperIndex < 9){
                    fprintf(fh,"%s;",segment);
                }
                if(segmentHelperIndex == 9) fprintf(fh,"%.3f",(float)atof(segment));
                added++;

            }
            segment = strtok(NULL, ";");
        }
        found = found ? 0 : found;
    }
    if(!globalFound) printf("No driver found with name %s", name);
   
    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
    if(fclose(fh) == EOF) printf("Subor sa nepodarilo zatvorit\n");
    
    if((fr = fopen("tabulka.csv", "w")) == NULL) printf("subor sa neotvoril\n");
    if((fh = fopen("helper.csv","r")) == NULL) printf("subor sa neotvoril\n");
    
    while(feof(fh) == 0){
        strcpy(row,"");
        fgets(row, MAXCHAR, fh);
        if(feof(fh) != 0 && strlen(row) == 0){
            return;
        }
        fprintf(fr,"%s",row);
    }
  
    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
    if(fclose(fh) == EOF) printf("Subor sa nepodarilo zatvorit\n");
}

void year(){
    FILE *fr;
    if((fr = fopen("tabulka.csv", "r")) == NULL) printf("subor sa neotvoril\n");
    char row[MAXCHAR];
    char *segment;
    char name[64];
    int year, yearLimiter, bestYear = 0;
    int bestRoundNum;
    char bestName[64];
    float bestRound = 0.0;
    int segmentHelperIndex = 0;
    float segmentRound;

    printf("Zadaj rok vo formate YYYY: ");
    if(!scanf(" %d", &yearLimiter) && year < 1000){
        printf("Nebol zadany spravny format, alebo bol zadany zly typ. Ukoncujem funkciu!");
        return;
    }

    while(feof(fr) == 0){
        fgets(row, MAXCHAR, fr);
        segment = strtok(row, ";");
        segmentHelperIndex = 0;
     
        while(segment != NULL){
            segmentHelperIndex++;
            if(segmentHelperIndex == 1) strcpy(name, segment);
         
            if(segmentHelperIndex == 3) year = (int)atoi(segment);
   
            if(segmentHelperIndex >= 3 && year < yearLimiter){
                if(!bestRound){
                    if(segmentHelperIndex == 5){
                        bestRound = (float)atof(segment);
                        bestRoundNum = 1;
                        bestYear = year;
                        strcpy(bestName,name);
                    }
                    if(segmentHelperIndex > 5){
                        segmentRound = (float)atof(segment);
                        if(segmentRound < bestRound){
                            bestRound = segmentRound;
                            bestRoundNum = segmentHelperIndex - 4;
                        }
                    }
                } else{
                    if(segmentHelperIndex > 4){
                        segmentRound = (float)atof(segment);
                        if(segmentRound < bestRound){
                            bestRound = segmentRound;
                            strcpy(bestName, name);
                            bestRoundNum = segmentHelperIndex - 4;
                            bestYear = year;
                        }
                    }
                }
            }
            segment = strtok(NULL, ";");
        }
    }
    if(bestYear) printf("\n%s\nnar. %d\nNajlepsie kolo: %.3f\nCislo kola: %d",bestName,bestYear,bestRound,bestRoundNum);
    else printf("No one found!\n");

    if(fclose(fr) == EOF) printf("Subor sa nepodarilo zatvorit\n");
}

void info(){
    printf("\n********************************");
    printf("\n  ZPrPr Projekt 1 ");
    printf("\n  Autor: Martin Orlej");
    printf("\n\n  Dostupne prikazy:");
    printf("\n  s - funkcia sum(), vypis vsetkych jazdcov");
    printf("\n  d - funkcia driver(), vypis hodnot podla zadaneho priezviska jazdca");
    printf("\n  l - funkcia lap(), vypis najlepsieho celkoveho kola");
    printf("\n  g - funkcia gender(), vypis najlepsieho kola podla zadaneho pohlavia");
    printf("\n  b - funkcia brand(), vypis najlepsieho kolo podla zadanej znacky vozidla");
    printf("\n  y - funkcia year(), vypis najlepsieho kola z jazdcov starsich ako zadany rok");
    printf("\n  a - funkcia average(), vypis najlepsieho priemerneho kola");
    printf("\n  u - funkcia under(), vypis jazdcov, ktory dali kolo pod zadany cas");
    printf("\n  c - funkcia change(), prepisanie hodnoty nejakeho kola");
    printf("\n  n - funkcia newdriver(), pridanie noveho jazdca a jeho hodnot");
    printf("\n  r - funkcia rmdriver(), vymazanie jazdca");
    printf("\n\n********************************");
}

void menu(){
    if(checkFileIntegrity()){
        char caller;
        int fix = 0;
        printf("Zadaj prikaz (zadaj i pre info): ");
        scanf("%c", &caller);
        while(getchar() != '\n'){
            fix++;
            if(fix == 1){
                printf("Bolo zadanych viacero znakov. Beriem len prvy znak!\n\n");
            }
        }
        fix = 0;
        while(caller != 'x' && checkFileIntegrity()){
            switch (caller)
            {
            case 'i':
                info();
                break;
            case 's':
                sum();
                break;
            case 'n':
                newdriver();
                break;
            case 'l':
                lap();
                break;
            case 'g':
                gender();
                break;
            case 'a':
                average();
                break;
            case 'd':
                driver();
                break;
            case 'y':
                year();
                break;
            case 'u':
                under();
                break;
            case 'b':
                brand();
                break;
            case 'c':
                change();
                break;
            case 'r':
                rmdriver();
                break;
            
            default:
                printf("Invalid function call\n");
            }
            printf("\n\nZadaj prikaz (zadaj i pre info): ");
            scanf(" %c", &caller);
            while(getchar() != '\n'){
                fix++;
                if(fix == 1){
                    printf("Bolo zadanych viacero znakov. Beriem len prvy znak!\n\n");
                }
            }
            fix = 0;
        }
        printf("\nSee you later, alligator!\n");
    } else printf("\nPlease fix the data file before using this program.\nHave a nice day!");
}
int main(){
    menu();
    return 0;
}