#include <Classes/Utils.h>

QString ReadFileString(const QString &filePath, bool* result)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        *result = false;
        return QString();
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    *result = true;
    return content;
}

QByteArray ReadFileBytearray(const QString &filePath, bool* result)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        *result = false;
        return {};
    }

    QByteArray content = file.readAll();
    file.close();

    *result = true;
    return content;
}

QString ValidCommandsFile(QByteArray jsonData, bool* result)
{
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError && document.isObject()) {
        *result = false;
        return QString("JSON parse error: %1").arg(parseError.errorString());
    }

    if(!document.isArray()) {
        *result = false;
        return "Error Commander Json Format";
    }

    QJsonArray commandArray = document.array();
    for (QJsonValue value : commandArray) {
        QJsonObject jsonObject = value.toObject();
        QString ret = ValidCommand(jsonObject, result);
        if( ! *result )
            return ret;
    }

    return "";
}

QString ValidCommand(QJsonObject extJsonObject, bool* result)
{
    QRegularExpression regex(R"((\w+)\s+([\[\<][^\s\]]+[\s\w-]*[\>\]])(\s*\([^\)]*\))?(?:\s+\{(.+)\})?)");

    if( !extJsonObject.contains("command") || !extJsonObject["command"].isString() ) {
        *result = false;
        return "Extension must include a required 'command' parameter";
    }
    QString command = extJsonObject["command"].toString();

    if (extJsonObject.contains("subcommands")) {
        QJsonArray subcommandArray = extJsonObject["subcommands"].toArray();
        for (QJsonValue subCmdVal : subcommandArray) {
            QJsonObject subCmdObj = subCmdVal.toObject();

            if( !subCmdObj.contains("name") || !subCmdObj["name"].isString() ) {
                *result = false;
                return QString("The command '%1' does not contain a 'name' parameter in Subcommand block").arg(command);
            }
            QString subcommand = subCmdObj["name"].toString();

            QJsonArray subArgsArray = subCmdObj["args"].toArray();
            for (QJsonValue subArgVal : subArgsArray) {
                QRegularExpressionMatch match = regex.match( subArgVal.toString() );
                if ( !match.hasMatch()) {
                    *result = false;
                    return "Arguments not parsed for command: " + command + " " + subcommand;
                }

                QString flagAndValue = match.captured(2).trimmed();
                if (!((flagAndValue.startsWith('<') && flagAndValue.endsWith('>')) || (flagAndValue.startsWith('[') && flagAndValue.endsWith(']')))) {
                    *result = false;
                    return "Argument must be in <> or [] for command: " + command + " " + subcommand;
                }
            }
        }
    } else {
        if (extJsonObject.contains("args")) {
            QJsonArray argsArray = extJsonObject["args"].toArray();
            for (QJsonValue argVal : argsArray) {
                QRegularExpressionMatch match = regex.match( argVal.toString() );
                if ( !match.hasMatch()) {
                    *result = false;
                    return "Arguments not parsed for command: " + command;
                }

                QString flagAndValue = match.captured(2).trimmed();
                if (!((flagAndValue.startsWith('<') && flagAndValue.endsWith('>')) || (flagAndValue.startsWith('[') && flagAndValue.endsWith(']')))) {
                    *result = false;
                    return "Argument must be in <> or [] for command: " + command;
                }
            }
        }
    }
    return "";
}

QString ValidExtCommand(QJsonObject extJsonObject, bool* result)
{
    QRegularExpression regex(R"((\w+)\s+([\[\<][^\s\]]+[\s\w-]*[\>\]])(\s*\([^\)]*\))?(?:\s+\{(.+)\})?)");

    if( !extJsonObject.contains("command") || !extJsonObject["command"].isString() ) {
        *result = false;
        return "Extension must include a required 'command' parameter";
    }
    QString command = extJsonObject["command"].toString();

    if (extJsonObject.contains("subcommands")) {
        QJsonArray subcommandArray = extJsonObject["subcommands"].toArray();
        for (QJsonValue subCmdVal : subcommandArray) {
            QJsonObject subCmdObj = subCmdVal.toObject();

            if( !subCmdObj.contains("name") || !subCmdObj["name"].isString() ) {
                *result = false;
                return QString("The command '%1' does not contain a 'name' parameter in Subcommand block").arg(command);
            }

            QString subcommand = subCmdObj["name"].toString();

            if( !subCmdObj.contains("exec") || !subCmdObj["exec"].isString() ) {
                *result = false;
                return "Extension must include a required 'exec' parameter for command: " + command + " " + subcommand;
            }

            QJsonArray subArgsArray = subCmdObj["args"].toArray();
            for (QJsonValue subArgVal : subArgsArray) {
                QRegularExpressionMatch match = regex.match( subArgVal.toString() );
                if ( !match.hasMatch()) {
                    *result = false;
                    return "Arguments not parsed for command: " + command + " " + subcommand;
                }

                QString flagAndValue = match.captured(2).trimmed();
                if (!((flagAndValue.startsWith('<') && flagAndValue.endsWith('>')) || (flagAndValue.startsWith('[') && flagAndValue.endsWith(']')))) {
                    *result = false;
                    return "Argument must be in <> or [] for command: " + command + " " + subcommand;
                }
            }
        }
    } else {
        if( !extJsonObject.contains("exec") || !extJsonObject["exec"].isString() ) {
            *result = false;
            return "Extension must include a required 'exec' parameter for command: " + command;
        }
        if (extJsonObject.contains("args")) {
            QJsonArray argsArray = extJsonObject["args"].toArray();
            for (QJsonValue argVal : argsArray) {
                QRegularExpressionMatch match = regex.match( argVal.toString() );
                if ( !match.hasMatch()) {
                    *result = false;
                    return "Arguments not parsed for command: " + command;
                }

                QString flagAndValue = match.captured(2).trimmed();
                if (!((flagAndValue.startsWith('<') && flagAndValue.endsWith('>')) || (flagAndValue.startsWith('[') && flagAndValue.endsWith(']')))) {
                    *result = false;
                    return "Argument must be in <> or [] for command: " + command;
                }
            }
        }
    }
    return "";
}

QString ValidExtConstant(QJsonObject extJsonObject, bool* result)
{
    if( !extJsonObject.contains("name") || !extJsonObject["name"].isString() ) {
        *result = false;
        return "Extension must include a required 'name' parameter";
    }
    QString mapName = extJsonObject["name"].toString();

    if ( !extJsonObject.contains("map") || !extJsonObject["map"].isObject() ) {
        *result = false;
        return "Extension must include a required 'map' parameter";
    }
    return "";
}

#include <QRandomGenerator>

QString GenerateRandomString(const int length, const QString &setName)
{
    QString characters;
    if (setName == "alphanumeric") {
        characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    } else if (setName == "alphabetic") {
        characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    } else if (setName == "numeric") {
        characters = "0123456789";
    } else {
        characters = "0123456789abcdef";
    }

    QString result;
    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(characters.length());
        result.append(characters.at(index));
    }
    return result;
}

#include <QCryptographicHash>

QString GenerateHash(const QString &algorithm, int length, const QString &inputString)
{
    QCryptographicHash::Algorithm hashAlgo;

    if (algorithm == "sha1") {
        hashAlgo = QCryptographicHash::Sha1;
    } else {
        hashAlgo = QCryptographicHash::Md5;
    }

    QByteArray hash = QCryptographicHash::hash(inputString.toUtf8(), hashAlgo).toHex();
    if (length > hash.size()) {
        length = hash.size();
    }
    return QString(hash.left(length));
}