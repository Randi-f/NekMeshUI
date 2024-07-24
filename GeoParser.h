/*
 * @Author: shihan
 * @Date: 2024-07-05 11:09:54
 * @version: 1.0
 * @description:
 */
#ifndef GEOPARSER_H
#define GEOPARSER_H

#include <unordered_map>
#include <memory>
#include <string>
#include "CADData.h"

class GeoParser {
public:
    GeoParser();
    void readGeoFile(const std::string &filename);
    std::shared_ptr<CADData> getCADData() const;

private:
    void parsePoint(const std::string &line);
    void parseLine(const std::string &line);
    void parseLineLoop(const std::string &line);
    void parseSpline(const std::string &line);
    std::shared_ptr<CADData> cadData;
};

#endif // GEOPARSER_H
