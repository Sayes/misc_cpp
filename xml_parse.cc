/*
 * Copyright (c) 2017-2018 S.Y.Z
 *
 * https://github.com/leethomason/tinyxml2.git
 * v6.0.0
 *
 * g++ xml_parse.cc -std=c++11 -I$TINYXML2_HOME/include -L$TINYXML2_HOME/lib
 * -ltinyxml2 -o release/xml_parse
 */

#include <tinyxml2.h>
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
  int result = -1;
  tinyxml2::XMLDocument* doc_ = nullptr;
  do {
    std::unique_ptr<tinyxml2::XMLDocument> doc_(new tinyxml2::XMLDocument());
    if (doc_ == nullptr) break;
    doc_->LoadFile(argv[1]);
    if (doc_->ErrorID()) break;

    tinyxml2::XMLElement* pElementRoot = doc_->FirstChildElement();
    if (pElementRoot) {
      std::string params = "{";
      tinyxml2::XMLElement* pElementSize =
          pElementRoot->FirstChildElement("size");
      if (pElementSize) {
        params += "\"size\":{";
        tinyxml2::XMLElement* pElementWidth =
            pElementSize->FirstChildElement("width");
        if (pElementWidth) {
          params += "\"width\":";
          params += pElementWidth->GetText();
        } else {
          params += "0";
        }
        params += ",";
        tinyxml2::XMLElement* pElementHeight =
            pElementSize->FirstChildElement("height");
        if (pElementHeight) {
          params += "\"height\":";
          params += pElementHeight->GetText();
        } else {
          params += "0";
        }
        params += ",";
        tinyxml2::XMLElement* pElementDepth =
            pElementSize->FirstChildElement("depth");
        if (pElementDepth) {
          params += "\"depth\":";
          params += pElementDepth->GetText();
        } else {
          params += "0";
        }
        params += "},";
      }
      tinyxml2::XMLElement* pElementObj =
          pElementRoot->FirstChildElement("object");
      params += "\"object\":[";
      while (pElementObj) {
        params += "{";
        tinyxml2::XMLElement* pElementName =
            pElementObj->FirstChildElement("name");
        if (pElementName) {
          params += "\"name\":\"";
          params += pElementName->GetText();
          params += "\"";

          tinyxml2::XMLElement* pElementBBox =
              pElementObj->FirstChildElement("bndbox");
          if (pElementBBox) {
            params += ",\"bndbox\":{";
            {
              tinyxml2::XMLElement* pElementXMin =
                  pElementBBox->FirstChildElement("xmin");
              if (pElementXMin) {
                params += "\"xmin\":";
                params += pElementXMin->GetText();
              }
              tinyxml2::XMLElement* pElementYMin =
                  pElementBBox->FirstChildElement("ymin");
              if (pElementYMin) {
                params += ",";

                params += "\"ymin\":";
                params += pElementYMin->GetText();
              }
              tinyxml2::XMLElement* pElementXMax =
                  pElementBBox->FirstChildElement("xmax");
              if (pElementXMax) {
                params += ",";
                params += "\"xmax\":";
                params += pElementXMax->GetText();
              }
              tinyxml2::XMLElement* pElementYMax =
                  pElementBBox->FirstChildElement("ymax");
              if (pElementYMax) {
                params += ",";
                params += "\"ymax\":";
                params += pElementYMax->GetText();
              }
            }
            params += "}";  // END bndbox
          }
        }
        params += "}";  // END object

        pElementObj = pElementObj->NextSiblingElement("object");
        if (pElementObj) {
          params += ",";
        }
      }
      params += "]";
      params += "}";

      std::cout << params << std::endl;
    }

    result = 0;
  } while (0);

  return result;
}
