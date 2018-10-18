/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOpcUa module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "testserver.h"
#include "qopen62541utils.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtCore/QVariant>
#include <QUuid>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    TestServer server;
    if (!server.init()) {
        qCritical() << "Could not initialize server.";
        return -1;
    }

    server.launch();

    int idx = server.registerNamespace(QLatin1String("http://qt-project.org"));
    if (idx != 2) {
        qWarning() << "Unexpected namespace index for qt-project namespace";
    }
    int ns2 = server.registerNamespace(QLatin1String("Test Namespace"));
    if (ns2 != 3) {
        qWarning() << "Unexpected namespace index for Test namespace";
    }

    const UA_NodeId largeTestFolder = server.addFolder("ns=1;s=Large.Folder", "Large_Folder");

    for (int x = 0; x < 100; ++x)
        server.addObject(largeTestFolder, idx, "");

    const UA_NodeId testFolder = server.addFolder("ns=3;s=TestFolder", "TestFolder");

    server.addVariable(testFolder, "ns=3;s=TestNode.ReadWrite", "TestNode.ReadWrite", 0.1, QOpcUa::Types::Double);

//    // TODO: Create Event

    // Test variables containing arrays of various types
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.Boolean", "BoolArrayTest", QVariantList({true, false}), QOpcUa::Types::Boolean);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.Byte", "ByteArrayTest", QVariantList({1, 2}), QOpcUa::Types::Byte);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.SByte", "SByteArrayTest", QVariantList({1, 2}), QOpcUa::Types::SByte);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.Double", "DoubleArrayTest", QVariantList({1.0, 2.0}), QOpcUa::Types::Double);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.Float", "FloatArrayTest", QVariantList({1.0f, 2.0f}), QOpcUa::Types::Float);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.Int16", "Int16ArrayTest", QVariantList({1, 2}), QOpcUa::Types::Int16);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.Int32", "Int32ArrayTest", QVariantList({1, 2}), QOpcUa::Types::Int32);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.Int64", "Int64ArrayTest", QVariantList({1, 2}), QOpcUa::Types::Int64);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.UInt16", "UInt16ArrayTest", QVariantList({1, 2}), QOpcUa::Types::UInt16);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.UInt32", "UInt32ArrayTest", QVariantList({1, 2}), QOpcUa::Types::UInt32);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.UInt64", "UInt64ArrayTest", QVariantList({1, 2}), QOpcUa::Types::UInt64);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.String", "StringArrayTest",
                       QVariantList({QStringLiteral("Value 1"), QStringLiteral("Value 2")}), QOpcUa::Types::String);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.LocalizedText", "LocalizedTextArrayTest",
                       QVariantList({QVariant::fromValue(QOpcUa::QLocalizedText(QStringLiteral("en-US"), QStringLiteral("Value 1"))),
                                     QVariant::fromValue(QOpcUa::QLocalizedText(QStringLiteral("en-US"), QStringLiteral("Value 2")))}),
                       QOpcUa::Types::LocalizedText);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.ByteString", "ByteStringArrayTest",
                       QVariantList({QByteArray("Value 1"), QByteArray("Value 2")}), QOpcUa::Types::ByteString);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.DateTime", "DateTimeArrayTest",
                       QVariantList({QDateTime::currentDateTime(), QDateTime::currentDateTime().addDays(1), QDateTime::currentDateTime().addDays(2)}),
                       QOpcUa::Types::DateTime);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.NodeId", "NodeIdArrayTest",
                       QVariantList({QStringLiteral("ns=0;i=84"), QStringLiteral("ns=0;i=85")}), QOpcUa::Types::NodeId);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.Guid", "GuidArrayTest",
                       QVariantList({QUuid::createUuid(), QUuid::createUuid()}), QOpcUa::Types::Guid);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.XmlElement", "XmlElementArrayTest",
                       QVariantList({QStringLiteral("<?xml version=\"1\" encoding=\"UTF-8\"?>"),
                                     QStringLiteral("<?xml version=\"1\" encoding=\"UTF-8\"?>")}), QOpcUa::Types::XmlElement);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.QualifiedName", "QualifiedNameArrayTest",
                       QVariantList({QVariant::fromValue(QOpcUa::QQualifiedName(0, QStringLiteral("Value 1"))),
                                     QVariant::fromValue(QOpcUa::QQualifiedName(0, QStringLiteral("Value 2")))}), QOpcUa::Types::QualifiedName);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.StatusCode", "StatusCodeArrayTest",
                       QVariantList({QVariant::fromValue(QOpcUa::UaStatusCode::Good),
                                     QVariant::fromValue(QOpcUa::UaStatusCode::BadInvalidArgument)}), QOpcUa::Types::StatusCode);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.Range", "RangeArrayTest",
                       QVariantList({QVariant::fromValue(QOpcUa::QRange(0, 100)),
                                     QVariant::fromValue(QOpcUa::QRange(100, 200))}), QOpcUa::Types::Range);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.EUInformation", "EUInformationArrayTest",
                       QVariantList({QVariant::fromValue(QOpcUa::QEUInformation()),
                                     QVariant::fromValue(QOpcUa::QEUInformation())}), QOpcUa::Types::EUInformation);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.ComplexNumber", "ComplexNumberArrayTest",
                       QVariantList({QVariant::fromValue(QOpcUa::QComplexNumber(1, 2)),
                                     QVariant::fromValue(QOpcUa::QComplexNumber(2, 3))}), QOpcUa::Types::ComplexNumber);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.DoubleComplexNumber", "DoubleComplexNumberArrayTest",
                       QVariantList({QVariant::fromValue(QOpcUa::QDoubleComplexNumber(1, 2)),
                                     QVariant::fromValue(QOpcUa::QDoubleComplexNumber(2, 3))}), QOpcUa::Types::DoubleComplexNumber);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.AxisInformation", "AxisInformationArrayTest",
                       QVariantList({QVariant::fromValue(QOpcUa::QAxisInformation()),
                                     QVariant::fromValue(QOpcUa::QAxisInformation())}), QOpcUa::Types::AxisInformation);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.XV", "XVArrayTest",
                       QVariantList({QVariant::fromValue(QOpcUa::QXValue(1, 2)),
                                     QVariant::fromValue(QOpcUa::QXValue(2, 3))}), QOpcUa::Types::XV);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.ExpandedNodeId", "ExpandedNodeIdArrayTest",
                       QVariantList({QOpcUa::QExpandedNodeId(QStringLiteral("ns1"), QStringLiteral("ns=0;i=64"), 1),
                                     QOpcUa::QExpandedNodeId(QString(), QStringLiteral("ns=1;i=84"))}),
                       QOpcUa::Types::ExpandedNodeId);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.Argument", "ArgumentArrayTest",
                       QVariantList({QOpcUa::QArgument(QStringLiteral("Argument1"), QStringLiteral("ns=0;i=12"), -1,
                                                       {},QOpcUa::QLocalizedText(QStringLiteral("en"), QStringLiteral("Description1"))),
                                     QOpcUa::QArgument(QStringLiteral("Argument2"), QStringLiteral("ns=0;i=12"), 2,
                                                       {2, 2}, QOpcUa::QLocalizedText(QStringLiteral("en"), QStringLiteral("Description2")))}),
                       QOpcUa::Types::Argument);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.ExtensionObject", "ExtensionObjectArrayTest",
                                                    QVariantList({QOpcUa::QExtensionObject(), QOpcUa::QExtensionObject()}),
                       QOpcUa::Types::ExtensionObject);

    // Test variables containing scalar values of various types
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.Boolean", "BoolScalarTest", false, QOpcUa::Types::Boolean);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.Byte", "ByteScalarTest", 1, QOpcUa::Types::Byte);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.SByte", "SByteScalarTest", 2, QOpcUa::Types::SByte);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.Double", "DoubleScalarTest", 1.0, QOpcUa::Types::Double);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.Float", "FloatScalarTest", 1.0f, QOpcUa::Types::Float);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.Int16", "Int16ScalarTest", 1, QOpcUa::Types::Int16);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.Int32", "Int32ScalarTest", 1, QOpcUa::Types::Int32);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.Int64", "Int64ScalarTest", 1, QOpcUa::Types::Int64);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.UInt16", "UInt16ScalarTest", 1, QOpcUa::Types::UInt16);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.UInt32", "UInt32ScalarTest", 1, QOpcUa::Types::UInt32);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.UInt64", "UInt64ScalarTest", 1, QOpcUa::Types::UInt64);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.String", "StringScalarTest", QStringLiteral("Value"), QOpcUa::Types::String);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.LocalizedText", "LocalizedTextScalarTest",
                                                  QVariant::fromValue(QOpcUa::QLocalizedText(QStringLiteral("en-US"), QStringLiteral("Value"))),
                       QOpcUa::Types::LocalizedText);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.ByteString", "ByteStringScalarTest", QByteArray("Value 1"), QOpcUa::Types::ByteString);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.DateTime", "DateTimeScalarTest", QDateTime::currentDateTime(), QOpcUa::Types::DateTime);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.NodeId", "NodeIdScalarTest", QStringLiteral("ns=0;i=84"), QOpcUa::Types::NodeId);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.Guid", "GuidScalarTest", QUuid::createUuid(), QOpcUa::Types::Guid);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.XmlElement", "XmlElementScalarTest",
                       QStringLiteral("<?xml version=\"1\" encoding=\"UTF-8\"?>"), QOpcUa::Types::XmlElement);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.QualifiedName", "QualifiedNameScalarTest",
                                                  QVariant::fromValue(QOpcUa::QQualifiedName(0, QStringLiteral("Value"))), QOpcUa::Types::QualifiedName);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.StatusCode", "StatusCodeScalarTest", QVariant::fromValue(QOpcUa::UaStatusCode::Good),
                       QOpcUa::Types::StatusCode);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.Range", "RangeScalarTest", QVariant::fromValue(QOpcUa::QRange(0, 100)), QOpcUa::Types::Range);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.EUInformation", "EUInformationScalarTest",
                                                    QVariant::fromValue(QOpcUa::QEUInformation()), QOpcUa::Types::EUInformation);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.ComplexNumber", "ComplexNumberArrayTest",
                                                    QVariant::fromValue(QOpcUa::QComplexNumber(1, 2)), QOpcUa::Types::ComplexNumber);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.DoubleComplexNumber", "DoubleComplexNumberScalarTest",
                                                    QVariant::fromValue(QOpcUa::QDoubleComplexNumber(1, 2)), QOpcUa::Types::DoubleComplexNumber);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.AxisInformation", "AxisInformationScalarTest",
                                                    QVariant::fromValue(QOpcUa::QAxisInformation()), QOpcUa::Types::AxisInformation);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.XV", "XVScalarTest",
                                                    QVariant::fromValue(QOpcUa::QXValue(1, 2)), QOpcUa::Types::XV);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.ExpandedNodeId", "ExpandedNodeIdScalarTest",
                                                    QOpcUa::QExpandedNodeId(QStringLiteral("ns1"), QStringLiteral("ns=0;i=64"), 1),
                       QOpcUa::Types::ExpandedNodeId);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.Argument", "ArgumentScalarTest",
                       QOpcUa::QArgument(QStringLiteral("Argument1"), QStringLiteral("ns=0;i=12"), -1,
                                         {},QOpcUa::QLocalizedText(QStringLiteral("en"), QStringLiteral("Description1"))),
                       QOpcUa::Types::Argument);
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Scalar.ExtensionObject", "ExtensionObjectScalarTest",
                                                    QOpcUa::QExtensionObject(), QOpcUa::Types::ExtensionObject);
    server.addNodeWithFixedTimestamp(testFolder, "ns=2;s=Demo.Static.FixedTimestamp", "FixedTimestamp");

    // Create folders containing child nodes with string, guid and opaque node ids
    UA_NodeId testStringIdsFolder = server.addFolder("ns=3;s=testStringIdsFolder", "testStringIdsFolder");
    server.addVariable(testStringIdsFolder, "ns=3;s=theStringId", "theStringId", QStringLiteral("Value"), QOpcUa::Types::String);
    UA_NodeId testGuidIdsFolder = server.addFolder("ns=3;s=testGuidIdsFolder", "testGuidIdsFolder");
    server.addVariable(testGuidIdsFolder, "ns=3;g=08081e75-8e5e-319b-954f-f3a7613dc29b", "theGuidId", QStringLiteral("Value"), QOpcUa::Types::String);
    UA_NodeId testOpaqueIdsFolder = server.addFolder("ns=3;s=testOpaqueIdsFolder", "testOpaqueIdsFolder");
    server.addVariable(testOpaqueIdsFolder, "ns=3;b=UXQgZnR3IQ==", "theOpaqueId", QStringLiteral("Value"), QOpcUa::Types::String);

    // Add a method to the test folder
    server.addMultiplyMethod(testFolder, "ns=3;s=Test.Method.Multiply", "MultiplyDoubles");
    server.addMultipleOutputArgumentsMethod(testFolder, "ns=3;s=Test.Method.MultipleOutputArguments", "MultipleOutputArguments");
    server.addEmptyArrayVariable(testFolder, "ns=2;s=EmptyBoolArray", "EmptyBoolArrayTest");

    const QVector<quint32> arrayDimensions({2, 2, 3});
    const QVariantList value({0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0});
    server.addVariable(testFolder, "ns=2;s=Demo.Static.Arrays.MultiDimensionalDouble", "MultiDimensionalDoubleTest",
                       QOpcUa::QMultiDimensionalArray(value, arrayDimensions), QOpcUa::Types::Double, QVector<quint32>({2, 2, 3}));

    // Add folders for relative nodes
    const UA_NodeId testFolder2 = server.addFolder("ns=3;s=TestFolder2", "TestFolder2");
    server.addVariable(testFolder2, "ns=3;s=TestNode2.ReadWrite", "TestNode.ReadWrite", 0.1, QOpcUa::Types::Double);

    // Add a method for adding namespaces
    server.addAddNamespaceMethod(Open62541Utils::nodeIdFromQString("ns=0;i=2253"), "ns=3;s=Test.Method.AddNamespace", "AddNamespace");

    // Add full writable node
    quint32 fullWritableMask = UA_WRITEMASK_ACCESSLEVEL | UA_WRITEMASK_ARRRAYDIMENSIONS | UA_WRITEMASK_BROWSENAME | UA_WRITEMASK_CONTAINSNOLOOPS |
                                UA_WRITEMASK_DATATYPE | UA_WRITEMASK_DESCRIPTION | UA_WRITEMASK_DISPLAYNAME | UA_WRITEMASK_EVENTNOTIFIER | UA_WRITEMASK_EXECUTABLE |
                                UA_WRITEMASK_HISTORIZING | UA_WRITEMASK_INVERSENAME | UA_WRITEMASK_ISABSTRACT | UA_WRITEMASK_MINIMUMSAMPLINGINTERVAL | UA_WRITEMASK_NODECLASS |
                                UA_WRITEMASK_NODEID | UA_WRITEMASK_SYMMETRIC | UA_WRITEMASK_USERACCESSLEVEL | UA_WRITEMASK_USEREXECUTABLE | UA_WRITEMASK_USERWRITEMASK |
                                UA_WRITEMASK_VALUERANK | UA_WRITEMASK_WRITEMASK | UA_WRITEMASK_VALUEFORVARIABLETYPE;

    server.addVariableWithWriteMask(testFolder, "ns=3;s=Demo.Static.Scalar.FullyWritable", "FullyWritableTest", 1.0, QOpcUa::Types::Double, fullWritableMask);

    return app.exec();
}
