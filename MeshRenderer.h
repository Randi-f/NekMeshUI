#ifndef MESHRENDERER_H
#define MESHRENDERER_H
#include <QApplication>
#include <QFile>
#include <QXmlStreamReader>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QString>

// class MeshRenderer : public QWidget {
// public:
//     explicit MeshRenderer(QWidget *parent = nullptr)
//         : QWidget(parent) {
//         loadMeshFromXml("/Users/shihan/Desktop/ADR_mesh.xml");
//     }

// protected:
//     void paintEvent(QPaintEvent *event) override {
//         Q_UNUSED(event);
//         QPainter painter(this);

//         // Example: Render mesh elements (you will need to adapt this based on your XML structure)
//         painter.setPen(Qt::black);
//         for (const auto &element : elements) {
//             // Draw each element based on its type (edges, triangles, etc.)
//             if (element.type == "edge") {
//                 painter.drawLine(element.points[0], element.points[1]);
//             } else if (element.type == "triangle") {
//                 QPolygonF polygon;
//                 polygon << element.points[0] << element.points[1] << element.points[2];
//                 painter.drawPolygon(polygon);
//             }
//             // Add more cases for other element types (quadrilaterals, higher-order shapes, etc.)
//         }
//     }

// private:
//     struct MeshElement {
//         QString type;
//         QVector<QPointF> points;
//     };

//     QVector<MeshElement> elements;

//     void loadMeshFromXml(const QString &fileName) {
//         QFile file(fileName);
//         if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//             qDebug() << "Failed to open XML file:" << fileName;
//             return;
//         }

//         QXmlStreamReader xmlReader(&file);

//         while (!xmlReader.atEnd() && !xmlReader.hasError()) {
//             QXmlStreamReader::TokenType token = xmlReader.readNext();

//             if (token == QXmlStreamReader::StartElement) {
//                 Qt::QStringRef elementName = xmlReader.name();
//                 if (elementName == "Node") {
//                     QPointF point;
//                     point.setX(xmlReader.attributes().value("X").toString().toDouble());
//                     point.setY(xmlReader.attributes().value("Y").toString().toDouble());
//                     // Add node processing as needed
//                 } else if (elementName == "Element") {
//                     MeshElement element;
//                     element.type = xmlReader.attributes().value("Type").toString();
//                     // Process element connectivity (nodes) and other attributes
//                     // Populate element.points based on connectivity
//                     // Add element to elements vector
//                 }
//             }
//         }

//         if (xmlReader.hasError()) {
//             qDebug() << "XML Error:" << xmlReader.errorString();
//         }

//         file.close();
//     }
// };
#endif // MESHRENDERER_H
