/*
 * @Author: shihan
 * @Date: 2024-07-05 11:10:12
 * @version: 1.0
 * @description:
 */
#include "GeoParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

GeoParser::GeoParser() : cadData(std::make_shared<CADData>()) {}

void GeoParser::readGeoFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        // if (keyword.find("Point") == 0) {
        //     parsePoint(line);
        // }else if (keyword.find("Line Loop") == 0) {
        //     parseLineLoop(line);
        // }
        // else if (keyword.find("Line") == 0) {
        //     parseLine(line);
        // }

        if (keyword.find("Point") == 0) {
            parsePoint(line);
        } else if (keyword.find("Line(") == 0 && keyword.find("Loop") == std::string::npos) {
            parseLine(line);
        } else if (keyword.find("Line Loop") == 0) {
            parseLineLoop(line);
        }
        // Add more parsing rules here for other entities like Spline, Line Loop, Plane Surface, etc.
    }

    // Debug output
    // std::cout << "Parsed points:" << std::endl;
    // for (const auto &point : cadData->points) {
    //     std::cout << "Point " << point.first << ": ("
    //               << point.second->x << ", "
    //               << point.second->y << ", "
    //               << point.second->z << ", "
    //               << point.second->lc << ")" << std::endl;
    // }

    // std::cout << "Parsed lines:" << std::endl;
    // for (const auto &line : cadData->lines) {
    //     std::cout << "Line " << line.first << ": ("
    //               << line.second->start << " -> "
    //               << line.second->end << ")" << std::endl;
    // }
}

void GeoParser::parsePoint(const std::string &line) {
    int id;
    auto point = std::make_shared<Point>();
    if (sscanf(line.c_str(), "Point(%d) = {%lf, %lf, %lf, %lf};", &id, &point->x, &point->y, &point->z, &point->lc) != 5) {
        std::cerr << "Error parsing point: " << line << std::endl;
        return;
    }
    cadData->points[id] = point;
}

void GeoParser::parseLine(const std::string &line) {
    int id, start, end;
    if (sscanf(line.c_str(), "Line(%d) = {%d, %d};", &id, &start, &end) != 3) {
        std::cerr << "Error parsing line: " << line << std::endl;
        return;
    }
    auto linePtr = std::make_shared<Line>();
    linePtr->start = start;
    linePtr->end = end;
    cadData->lines[id] = linePtr;
}

void GeoParser::parseLineLoop(const std::string &line){
    int id;
    std::string lineList;
    if (sscanf(line.c_str(), "Line Loop(%d) = {%[^}]", &id, &lineList[0]) != 2) {
        std::cerr << "Error parsing line loop: " << line << std::endl;
        return;
    }

    auto lineLoopPtr = std::make_shared<LineLoop>();
    std::istringstream iss(lineList);
    std::string lineId;
    while (std::getline(iss, lineId, ',')) {
        lineLoopPtr->lines.push_back(std::stoi(lineId));
    }
    cadData->lineLoops[id] = lineLoopPtr;
}

void GeoParser::parseSpline(const std::string &line){
    int id;
    std::string pointList;
    if (sscanf(line.c_str(), "Line Loop(%d) = {%[^}]", &id, &pointList[0]) != 2) {
        std::cerr << "Error parsing spline: " << line << std::endl;
        return;
    }

    auto splinePtr = std::make_shared<Spline>();
    std::istringstream iss(pointList);
    std::string splineId;
    while (std::getline(iss, splineId, ',')) {
        splinePtr->points.push_back(std::stoi(splineId));
    }
    cadData->splines[id] = splinePtr;
}
std::shared_ptr<CADData> GeoParser::getCADData() const {
    return cadData;
}
