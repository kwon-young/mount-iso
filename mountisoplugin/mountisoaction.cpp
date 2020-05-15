/*
 * Copyright (C) 2020 Kwon-Young Choi <kwon-young.choi@hotmail.fr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "mountisoaction.h"

#include <QIcon>
#include <QAction>
#include <QProcess>

#include <KPluginFactory>
#include <KLocalizedString>

K_PLUGIN_CLASS_WITH_JSON(MountIsoAction, "mountisoaction.json")

MountIsoAction::MountIsoAction(QObject* parent, const QVariantList&)
    : KAbstractFileItemActionPlugin(parent)
{}

QList<QAction*> MountIsoAction::actions(const KFileItemListProperties& fileItemInfos, QWidget* parentWidget)
{
    if (fileItemInfos.urlList().size() != 1 ||
        fileItemInfos.mimeType() != QString("application/x-cd-image") ||
        !fileItemInfos.isLocal()) return {};

    QList<QAction*> actions;

    auto fileItem = fileItemInfos.urlList()[0].toLocalFile();

    // command to get the loopback block device /dev/loop# associated with an
    // iso file
    auto losetup_command = QString("losetup -j %1").arg(fileItem);
    QProcess process;
    process.start(losetup_command);
    process.waitForFinished(-1);

    QString losetup_output = process.readAllStandardOutput();
    // losetup will either return nothing if there is no loopback device or
    // a line like this:
    // /dev/loop0: []: (/home/user/Downloads/myimage.iso)
    QStringList loopbackDeviceInfo = losetup_output.split(':', QString::SkipEmptyParts);

    auto createAction = [this] (const QIcon& icon, const QString& title,
                                const QString& command, QWidget* parentWidget)
    {
        QAction *action = new QAction(icon, title, parentWidget);

        connect(action, &QAction::triggered, this, [command]() {
            QProcess process;
            process.start(command);
            process.waitForFinished(-1);
        });

        return action;
    };

    if (loopbackDeviceInfo.size() == 0) {
        // losetup returned nothing, so mount the iso
        const QIcon icon = QIcon::fromTheme(QStringLiteral("media-mount"));
        const QString title = 
            i18nc("@action Action to mount an iso image", "Mount this iso image");
        const auto command = QString("udisksctl loop-setup -r -f %1").arg(fileItem);
        return {createAction(icon, title, command, parentWidget)};
    } else if (loopbackDeviceInfo.size() > 0){
        // losetup returned something, so loopbackDeviceInfo[0] is the loopback
        // block device associated with fileItem
        QString loopbackDevice = loopbackDeviceInfo[0];
        const QIcon icon = QIcon::fromTheme(QStringLiteral("media-eject"));
        const QString title = 
            i18nc("@action Action to unmount an iso image", "Unmount this iso image");
        const auto command = QString("udisksctl loop-delete -b %1").arg(loopbackDevice);
        return {createAction(icon, title, command, parentWidget)};
    }

    return {};
}

#include "mountisoaction.moc"
