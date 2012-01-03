//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "stok.h"
#include "anamenu.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmStok *frmStok;
//---------------------------------------------------------------------------
__fastcall TfrmStok::TfrmStok(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmStok::ToolButton9Click(TObject *Sender)
{
    tbtnAC->Visible = true;
    tbtnKAPA->Visible = false;
    GroupBox2->Visible = false;
    frmStok->Width = 340;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::Sorgula(void)
{
    querySTOKLISTE->Close();
    querySTOKLISTE->ParamByName("URUNKODU")->AsString = "%";
    querySTOKLISTE->ParamByName("URUNADI")->AsString = "%";
    querySTOKLISTE->Open();
}

void __fastcall TfrmStok::tbtnACClick(TObject *Sender)
{
    tbtnAC->Visible = false;
    tbtnKAPA->Visible = true;
    frmStok->Width = 603;
    GroupBox2->Visible = true;
    ARAMA_Edit->Clear();
    Sorgula();
}
//---------------------------------------------------------------------------


void __fastcall TfrmStok::tbtnYeniClick(TObject *Sender)
{
    KayitKontrol();
    Anahtar();
    STOKKODU_Edit->Enabled = true;
    STOKKODU_Edit->Color = clWhite;
    tblSTOKANA->Filtered = false;
    tblSTOKANA->Insert();
    SonDurum();
    tbtnKaydet->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::tbtnKaydetClick(TObject *Sender)
{
    if (STOKKODU_Edit->Text.Length() == 0)
        Application->MessageBoxA("Stok kod numaras� girmelisiniz!",
            "Uyar� Mesaj�", MB_OK + MB_ICONEXCLAMATION);
    else if (STOKADI_Edit->Text.Length() == 0)
        Application->MessageBoxA("�r�n�n ad�n� girmelisiniz!",
            "Uyar� Mesaj�", MB_OK + MB_ICONEXCLAMATION);
    else {
        tblSTOKANA->Post();
        tbtnKaydet->Enabled = false;
        Sorgula();
        STOKKODU_Edit->Enabled = false;
        STOKKODU_Edit->Color = cl3DLight;
    }
}

void __fastcall TfrmStok::KayitKontrol(void)
{
    int nDeger;
    TObject *Sender;

    if (tbtnKaydet->Enabled == true) {
        nDeger = Application->MessageBox("De�i�iklikler kaydedilsin mi?",
            "Uyar� mesaj�", MB_YESNO + MB_ICONQUESTION);

        if (nDeger == IDYES)
            tbtnKaydetClick(Sender);
        else
            tblSTOKANA->Cancel();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::tbtnSilClick(TObject *Sender)
{
    int nDeger;

    if (tblSTOKANA->RecordCount <= 0)
        Application->MessageBoxA("Silecek bir kay�t kalmad�",
            "Uyar� Mesaj�", MB_OK + MB_ICONEXCLAMATION);
    else if (queryCIKIS->RecordCount > 0 || queryGIRIS->RecordCount > 0)
        Application->MessageBoxA("�nce hareketleri silmelisiniz",
            "Uyar� Mesaj�", MB_OK + MB_ICONEXCLAMATION);
    else {
        nDeger = Application->MessageBox("Kayd� silmek son karar�n�z m�?", "Silme onay�",
            MB_YESNO + MB_ICONQUESTION);

        if (nDeger == IDYES) {
            tblSTOKANA->Delete();
            Kilit();
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::ARAMA_EditChange(TObject *Sender)
{
    querySTOKLISTE->Close();
    querySTOKLISTE->ParamByName("URUNKODU")->AsString = ARAMA_Edit->Text + "%";
    querySTOKLISTE->ParamByName("URUNADI")->AsString = ARAMA_Edit->Text + "%";
    querySTOKLISTE->Open();
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::SonDurum(void)
{
    double Giris_Toplami = 0, Cikis_Toplami = 0, Son_Durum = 0;

//------------ stok son durum miktar� ayar� -----------
    queryGIRIS->Close();
    queryGIRIS->ParamByName("URUNKODU")->AsString = STOKKODU_Edit->Text;
    queryGIRIS->Open();

    queryGIRIS->First();
    while(!queryGIRIS->Eof) {
        Giris_Toplami += queryGIRISMAL_ADEDI->Value;
        queryGIRIS->Next();
    }
//-----------------------------------------------------
    queryCIKIS->Close();
    queryCIKIS->ParamByName("URUNKODU")->AsString = STOKKODU_Edit->Text;
    queryCIKIS->Open();

    queryCIKIS->First();
    while(!queryCIKIS->Eof) {
        Cikis_Toplami += queryCIKISMAL_ADEDI->Value;
        queryCIKIS->Next();
    }

    if (MIKTAR_Edit->Text.Length() != 0)
        Son_Durum = (StrToInt(MIKTAR_Edit->Text) + Giris_Toplami) - Cikis_Toplami;
    else
        Son_Durum = (tblSTOKANAMIKTARI->Value + Giris_Toplami) - Cikis_Toplami;
    SONDURUM_Edit->Text = FloatToStrF(Son_Durum, ffNumber, 10, 0);
}
void __fastcall TfrmStok::DBGrid3CellClick(TColumn *Column)
{
    AnsiString AKTARMA;
    AKTARMA = DBGrid3->Columns->Grid->SelectedField->AsString;

    Anahtar();
    tblSTOKANA->Edit();
    tblSTOKANA->Filtered = true;
    tblSTOKANA->Filter = "[URUNKODU] ='"+AKTARMA+"'";
    tblSTOKANA->Edit();
    STOKKODU_Edit->Enabled = false;
    STOKKODU_Edit->Color = cl3DLight;
    tbtnKaydet->Enabled = false;
    SonDurum();
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::STOKKODU_EditChange(TObject *Sender)
{
    tbtnKaydet->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::STOKADI_EditChange(TObject *Sender)
{
    tbtnKaydet->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::FIYATI_EditChange(TObject *Sender)
{
    tbtnKaydet->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::MIKTAR_EditChange(TObject *Sender)
{
    tbtnKaydet->Enabled = true;
    SonDurum();
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::STOKKODU_EditKeyPress(TObject *Sender, char &Key)
{
    if (Key == Char(VK_RETURN)) {
        Key = 0;
        PostMessage(Handle, WM_NEXTDLGCTL, 0, 0);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::STOKADI_EditKeyPress(TObject *Sender, char &Key)
{
    if (Key == Char(VK_RETURN)) {
        Key = 0;
        PostMessage(Handle, WM_NEXTDLGCTL, 0, 0);
    }    
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::FIYATI_EditKeyPress(TObject *Sender, char &Key)
{
    if (Key == Char(VK_RETURN)) {
        Key = 0;
        PostMessage(Handle, WM_NEXTDLGCTL, 0, 0);
    }    
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::MIKTAR_EditKeyPress(TObject *Sender, char &Key)
{
    if (Key == Char(VK_RETURN)) {
        Key = 0;
        PostMessage(Handle, WM_NEXTDLGCTL, 0, 0);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::FormClose(TObject *Sender, TCloseAction &Action)
{
    KayitKontrol();
    Kilit();
    ToolButton9Click(Sender);
    frmAnaMenu->Show();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmStok::Kilit(void)
{
//----------------------------------
    queryCIKIS->Close();
    queryGIRIS->Close();
    tblSTOKANA->Close();
//----------------------------------

    STOKKODU_Edit->Enabled = false;
    STOKADI_Edit->Enabled = false;
    FIYATI_Edit->Enabled = false;
    MIKTAR_Edit->Enabled = false;
    SONDURUM_Edit->Enabled = false;
    tbtnKaydet->Enabled = false;
    tbtnSil->Enabled = false;
    DBGrid1->Enabled = false;
    DBGrid2->Enabled = false;

    STOKKODU_Edit->Color = clGray;
    STOKADI_Edit->Color = clGray;
    FIYATI_Edit->Color = clGray;
    MIKTAR_Edit->Color = clGray;
    SONDURUM_Edit->Color = clGray;
    DBGrid1->Color = clGray;
    DBGrid2->Color = clGray;
}

void __fastcall TfrmStok::Anahtar(void)
{
//----------------------------------
    queryCIKIS->Open();
    queryGIRIS->Open();
    tblSTOKANA->Open();
//----------------------------------

    STOKADI_Edit->Enabled = true;
    FIYATI_Edit->Enabled = true;
    tbtnSil->Enabled = true;
    MIKTAR_Edit->Enabled = true;
    DBGrid1->Enabled = true;
    DBGrid2->Enabled = true;

    STOKKODU_Edit->Color = clWhite;
    STOKADI_Edit->Color = clWhite;
    FIYATI_Edit->Color = clWhite;
    MIKTAR_Edit->Color = clWhite;
    SONDURUM_Edit->Color = clWhite;
    DBGrid1->Color = clWhite;
    DBGrid2->Color = clWhite;
}

void __fastcall TfrmStok::FormShow(TObject *Sender)
{
    Kilit();
    frmStok->Width = 340;
    SonDurum();
}
//---------------------------------------------------------------------------

