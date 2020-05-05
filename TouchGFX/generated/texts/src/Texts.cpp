/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#include <stdarg.h>
#include <touchgfx/Texts.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/TypedText.hpp>
#include <texts/TypedTextDatabase.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/TextProvider.hpp>

touchgfx::Font::StringWidthFunctionPointer touchgfx::Font::getStringWidthFunction = &touchgfx::Font::getStringWidthLTR;
touchgfx::LCD::DrawStringFunctionPointer touchgfx::LCD::drawStringFunction = &touchgfx::LCD::drawStringLTR;
touchgfx::TextProvider::UnicodeConverterInitFunctionPointer touchgfx::TextProvider::unicodeConverterInitFunction = static_cast<touchgfx::TextProvider::UnicodeConverterInitFunctionPointer>(0);
touchgfx::TextProvider::UnicodeConverterFunctionPointer touchgfx::TextProvider::unicodeConverterFunction = static_cast<touchgfx::TextProvider::UnicodeConverterFunctionPointer>(0);

//Default typed text database
extern const touchgfx::TypedText::TypedTextData* const typedTextDatabaseArray[];

TEXT_LOCATION_FLASH_PRAGMA
KEEP extern const touchgfx::Unicode::UnicodeChar texts_all_languages[] TEXT_LOCATION_FLASH_ATTRIBUTE =
{
    0x54, 0x65, 0x6d, 0x70, 0x65, 0x72, 0x61, 0x74, 0x75, 0x72, 0x65, 0x3a, 0x20, 0x2, 0x20, 0x43, 0x0, // @0 "Temperature: <> C"
    0x41, 0x76, 0x67, 0x53, 0x70, 0x65, 0x65, 0x64, 0x3a, 0x20, 0x2, 0x20, 0x6b, 0x6d, 0x2f, 0x68, 0x0, // @17 "AvgSpeed: <> km/h"
    0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x73, 0x3a, 0x20, 0x2, 0x0, // @34 "Output files: <>"
    0x50, 0x72, 0x65, 0x73, 0x73, 0x75, 0x72, 0x65, 0x3a, 0x20, 0x2, 0x20, 0x68, 0x50, 0x61, 0x0, // @50 "Pressure: <> hPa"
    0x4c, 0x61, 0x74, 0x3a, 0x20, 0x2, 0x20, 0x2, 0x0, // @66 "Lat: <> <>"
    0x44, 0x69, 0x73, 0x74, 0x61, 0x6e, 0x63, 0x65, 0x3a, 0x20, 0x2, 0x20, 0x6b, 0x6d, 0x0, // @75 "Distance: <> km"
    0x54, 0x6f, 0x74, 0x61, 0x6c, 0x20, 0x73, 0x70, 0x61, 0x63, 0x65, 0x3a, 0x20, 0x2, 0x0, // @90 "Total space: <>"
    0x4c, 0x6f, 0x6e, 0x3a, 0x20, 0x2, 0x20, 0x2, 0x0, // @105 "Lon: <> <>"
    0x49, 0x6e, 0x70, 0x75, 0x74, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x73, 0x3a, 0x20, 0x2, 0x0, // @114 "Input files: <>"
    0x46, 0x72, 0x65, 0x65, 0x20, 0x73, 0x70, 0x61, 0x63, 0x65, 0x3a, 0x20, 0x2, 0x0, // @129 "Free space: <>"
    0x41, 0x6c, 0x74, 0x69, 0x74, 0x75, 0x64, 0x65, 0x3a, 0x20, 0x2, 0x20, 0x6d, 0x0, // @143 "Altitude: <> m"
    0x44, 0x65, 0x62, 0x75, 0x67, 0x20, 0x45, 0x6e, 0x76, 0x53, 0x65, 0x6e, 0x73, 0x6f, 0x72, 0x73, 0x0, // @157 "Debug EnvSensors"
    0x41, 0x6c, 0x74, 0x69, 0x3a, 0x20, 0x2, 0x20, 0x6d, 0x0, // @174 "Alti: <> m"
    0x53, 0x74, 0x61, 0x74, 0x65, 0x3a, 0x20, 0x2, 0x0, // @184 "State: <>"
    0x44, 0x65, 0x62, 0x75, 0x67, 0x20, 0x47, 0x70, 0x73, 0x20, 0x44, 0x61, 0x74, 0x61, 0x0, // @193 "Debug Gps Data"
    0x54, 0x69, 0x6d, 0x65, 0x72, 0x3a, 0x20, 0x2, 0x0, // @208 "Timer: <>"
    0x57, 0x72, 0x69, 0x74, 0x65, 0x3a, 0x20, 0x2, 0x0, // @217 "Write: <>"
    0x53, 0x74, 0x61, 0x72, 0x74, 0x20, 0x61, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79, 0x0, // @226 "Start activity"
    0x44, 0x61, 0x74, 0x65, 0x3a, 0x20, 0x2, 0x0, // @241 "Date: <>"
    0x54, 0x69, 0x6d, 0x65, 0x3a, 0x20, 0x2, 0x0, // @249 "Time: <>"
    0x44, 0x65, 0x62, 0x75, 0x67, 0x20, 0x53, 0x44, 0x20, 0x63, 0x61, 0x72, 0x64, 0x0, // @257 "Debug SD card"
    0x52, 0x65, 0x61, 0x64, 0x3a, 0x20, 0x2, 0x0, // @271 "Read: <>"
    0x55, 0x4e, 0x49, 0x4e, 0x49, 0x54, 0x49, 0x41, 0x4c, 0x49, 0x5a, 0x45, 0x44, 0x0, // @279 "UNINITIALIZED"
    0x46, 0x69, 0x78, 0x3a, 0x20, 0x2, 0x0, // @293 "Fix: <>"
    0x53, 0x61, 0x74, 0x3a, 0x20, 0x2, 0x0, // @300 "Sat: <>"
    0x70, 0x61, 0x74, 0x68, 0x54, 0x72, 0x61, 0x63, 0x6b, 0x65, 0x72, 0x0, // @307 "pathTracker"
    0x41, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x0, // @319 "Application"
    0x30, 0x31, 0x2e, 0x30, 0x31, 0x2e, 0x32, 0x30, 0x32, 0x30, 0x0, // @331 "01.01.2020"
    0x30, 0x30, 0x3a, 0x30, 0x30, 0x3a, 0x30, 0x30, 0x2e, 0x30, 0x0, // @342 "00:00:00.0"
    0x44, 0x65, 0x62, 0x75, 0x67, 0x20, 0x44, 0x72, 0x61, 0x77, 0x0, // @353 "Debug Draw"
    0x41, 0x76, 0x67, 0x20, 0x73, 0x70, 0x65, 0x65, 0x64, 0x0, // @364 "Avg speed"
    0x53, 0x6c, 0x6f, 0x70, 0x65, 0x20, 0x6d, 0x61, 0x78, 0x0, // @374 "Slope max"
    0x4d, 0x61, 0x78, 0x20, 0x73, 0x70, 0x65, 0x65, 0x64, 0x0, // @384 "Max speed"
    0x41, 0x6c, 0x74, 0x69, 0x20, 0x64, 0x6f, 0x77, 0x6e, 0x0, // @394 "Alti down"
    0x41, 0x6c, 0x74, 0x69, 0x20, 0x6d, 0x61, 0x78, 0x0, // @404 "Alti max"
    0x4e, 0x65, 0x77, 0x20, 0x54, 0x65, 0x78, 0x74, 0x0, // @413 "New Text"
    0x53, 0x65, 0x74, 0x74, 0x69, 0x6e, 0x67, 0x73, 0x0, // @422 "Settings"
    0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79, 0x0, // @431 "Activity"
    0x41, 0x6c, 0x74, 0x69, 0x74, 0x75, 0x64, 0x65, 0x0, // @440 "Altitude"
    0x31, 0x32, 0x3a, 0x30, 0x30, 0x3a, 0x30, 0x30, 0x0, // @449 "12:00:00"
    0x44, 0x69, 0x73, 0x74, 0x61, 0x6e, 0x63, 0x65, 0x0, // @458 "Distance"
    0x30, 0x30, 0x3a, 0x30, 0x30, 0x3a, 0x30, 0x30, 0x0, // @467 "00:00:00"
    0x41, 0x6c, 0x74, 0x69, 0x20, 0x75, 0x70, 0x0, // @476 "Alti up"
    0x43, 0x79, 0x63, 0x6c, 0x69, 0x6e, 0x67, 0x0, // @484 "Cycling"
    0x53, 0x70, 0x65, 0x65, 0x64, 0x0, // @492 "Speed"
    0x43, 0x6c, 0x6f, 0x63, 0x6b, 0x0, // @498 "Clock"
    0x6b, 0x6d, 0xa, 0x20, 0x68, 0x0, // @504 "km? h"
    0x53, 0x6c, 0x6f, 0x70, 0x65, 0x0, // @510 "Slope"
    0x44, 0x65, 0x62, 0x75, 0x67, 0x0, // @516 "Debug"
    0x54, 0x69, 0x6d, 0x65, 0x72, 0x0, // @522 "Timer"
    0x30, 0x2e, 0x30, 0x30, 0x0, // @528 "0.00"
    0x2d, 0x2d, 0x2d, 0x0, // @533 "---"
    0x25, 0x0 // @537 "%"
};
extern uint32_t const indicesGb[];

//array holding dynamically installed languages
struct TranslationHeader
{
    uint32_t offset_to_texts;
    uint32_t offset_to_indices;
    uint32_t offset_to_typedtext;
};
static const TranslationHeader* languagesArray[1] = { 0 };

//Compiled and linked in languages
static const uint32_t* const staticLanguageIndices[] =
{
    indicesGb
};

touchgfx::LanguageId touchgfx::Texts::currentLanguage = static_cast<touchgfx::LanguageId>(0);
static const touchgfx::Unicode::UnicodeChar* currentLanguagePtr = 0;
static const uint32_t* currentLanguageIndices = 0;

void touchgfx::Texts::setLanguage(touchgfx::LanguageId id)
{
    const touchgfx::TypedText::TypedTextData* currentLanguageTypedText = 0;
    if (id < 1)
    {
        if (languagesArray[id] != 0)
        {
            //dynamic translation is added
            const TranslationHeader* translation = languagesArray[id];
            currentLanguagePtr = (const touchgfx::Unicode::UnicodeChar*)(((const uint8_t*)translation) + translation->offset_to_texts);
            currentLanguageIndices = (const uint32_t*)(((const uint8_t*)translation) + translation->offset_to_indices);
            currentLanguageTypedText = (const touchgfx::TypedText::TypedTextData*)(((const uint8_t*)translation) + translation->offset_to_typedtext);
        }
        else
        {
            //compiled and linked in languages
            currentLanguagePtr = texts_all_languages;
            currentLanguageIndices = staticLanguageIndices[id];
            currentLanguageTypedText = typedTextDatabaseArray[id];
        }
    }

    if (currentLanguageTypedText)
    {
        currentLanguage = id;
        touchgfx::TypedText::registerTypedTextDatabase(currentLanguageTypedText,
                                                       TypedTextDatabase::getFonts(), TypedTextDatabase::getInstanceSize());
    }
}

void touchgfx::Texts::setTranslation(touchgfx::LanguageId id, const void* translation)
{
    languagesArray[id] = (const TranslationHeader*)translation;
}

const touchgfx::Unicode::UnicodeChar* touchgfx::Texts::getText(TypedTextId id) const
{
    return &currentLanguagePtr[currentLanguageIndices[id]];
}

