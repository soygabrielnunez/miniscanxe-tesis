#include "dlgmodificarlesion.h"
#include "ui_dlgmodificarlesion.h"

dlgModificarLesion::dlgModificarLesion(QString claveUsuario, QHash<QString, QString> infoLesion, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgModificarLesion)
{
    ui->setupUi(this);

    clave = claveUsuario;
    infoOriginal = infoLesion;
    infoNueva.clear();

    QRegExp rx("^([a-zA-Z]+([ ]?[a-zA-Z]?[a-zA-Z]+)*)$");
    QValidator *soloPalabras = new QRegExpValidator(rx, this);
    ui->lineaNombre->setValidator(soloPalabras);
    ui->lineaArea->setValidator(soloPalabras);

    ui->lineaNombre->setText(infoLesion["nombre_muestra"]);
    ui->lineaArea->setText(infoLesion["area_muestra"]);

    if(infoLesion.contains("observaciones")){
        ui->textEditObservaciones->setPlainText(infoLesion["observaciones"]);
    }

    this->setFixedSize(this->size());
    this->setWindowFlags(Qt::WindowCloseButtonHint);
}

dlgModificarLesion::~dlgModificarLesion()
{
    delete ui;
}

void dlgModificarLesion::on_btnCancelar_clicked()
{
    close();
}

void dlgModificarLesion::on_lineaNombre_textChanged(const QString &arg1)
{
    ui->lineaNombre->setText(arg1.toUpper());

    if(!ui->lineaNombre->text().isEmpty() && ui->lineaNombre->text() != infoOriginal["nombre_muestra"]){
        infoNueva["nombre_muestra"] = ui->lineaNombre->text();
    }else{
        if(infoNueva.contains("nombre_muestra")){
            infoNueva.remove("nombre_muestra");
        }
    }

    if(!infoNueva.isEmpty()){
        ui->btnModificar->setEnabled(true);
    }else{
        ui->btnModificar->setEnabled(false);
    }
}

void dlgModificarLesion::on_lineaArea_textChanged(const QString &arg1)
{
    ui->lineaArea->setText(arg1.toUpper());

    if(!ui->lineaArea->text().isEmpty() && ui->lineaArea->text() != infoOriginal["area_muestra"]){
        infoNueva["area_muestra"] = ui->lineaArea->text();
    }else{
        if(infoNueva.contains("area_muestra")){
            infoNueva.remove("area_muestra");
        }
    }

    if(!infoNueva.isEmpty()){
        ui->btnModificar->setEnabled(true);
    }else{
        ui->btnModificar->setEnabled(false);
    }
}

void dlgModificarLesion::on_textEditObservaciones_textChanged()
{
    if(infoOriginal.contains("observaciones")){

        if(ui->textEditObservaciones->toPlainText() != infoOriginal["observaciones"]){
            infoNueva["observaciones"] = ui->textEditObservaciones->toPlainText();
        }else{
            if(infoNueva.contains("observaciones")){
                infoNueva.remove("observaciones");
            }
        }
    }else{

        if(!ui->textEditObservaciones->toPlainText().isEmpty()){
            infoNueva["observaciones"] = ui->textEditObservaciones->toPlainText();
        }else{
            if(infoNueva.contains("observaciones")){
                infoNueva.remove("observaciones");
            }
        }
    }

    if(!infoNueva.isEmpty()){
        ui->btnModificar->setEnabled(true);
    }else{
        ui->btnModificar->setEnabled(false);
    }
}

void dlgModificarLesion::on_btnModificar_clicked()
{
    dlgConfirmarClave confirmar(clave);

    connect(&confirmar, &dlgConfirmarClave::claveIntroducida, this, &dlgModificarLesion::on_claveIntroducida);

    confirmar.exec();
}

void dlgModificarLesion::on_claveIntroducida(bool correcta)
{
    if(correcta){
        QString consulta = "UPDATE spectradb.muestra SET ";
        bool varios = false;

        if(infoNueva.contains("nombre_muestra")){
            consulta+= "nombre_muestra = '" + infoNueva["nombre_muestra"] + "'";
            varios = true;
        }

        if(infoNueva.contains("area_muestra")){
            if(varios){
                consulta+= ", ";
            }
            consulta+= "area_muestra = '" + infoNueva["area_muestra"] + "'";
            varios = true;
        }

        if(infoNueva.contains("observaciones")){
            if(varios){
                consulta+= ", ";
            }
            consulta+= "observaciones = ";

            if(infoNueva["observaciones"] == ""){
                consulta+= "NULL";
            }else{
                consulta+= "'" + infoNueva["observaciones"] + "'";
            }
        }

        consulta+= " WHERE id_muestra = '" + infoOriginal["id_muestra"] + "'";

        QSqlQuery query;

        query.prepare(consulta);

        if(query.exec()){

            QMessageBox::information(this, "Muestra modificada", "Se ha modificado la muestra correctamente.");

            close();

            QHash<QString, QString> infoResultante;
            QStringList llaves, valores;

            llaves = infoNueva.keys();
            valores = infoNueva.values();

            infoResultante = infoOriginal;

            for(int i = 0; i < infoNueva.size(); ++i){
                infoResultante[llaves[i]] = valores[i];
            }

            emit lesionModificada(infoResultante);
        }else{
            qDebug() << query.lastError();
        }

    }else{
        QMessageBox::critical(this, "Contraseña incorrecta", "La contraseña que introdujo es incorrecta.");
    }
}
