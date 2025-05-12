#include "esp8266.h"

enum { GET_DATA_MAX_LENGTH = 512, SEND_DATA_MAX_LENGTH = 2048, SUBSTRING_LENGTH = 7, COMMAND_LENGTH = 64 };

const char *const html =
    "<!DOCTYPE html><html><head><meta charset=\"UTF-8\" /><meta name=\"viewport\" content=\"width=device-width, "
    "initial-scale=1.0\" /><title>Контроль мікроклімату</title><style>/*! tailwindcss v4.1.6 | MIT License | "
    "https://tailwindcss.com */@layer properties{@supports (((-webkit-hyphens:none)) and (not (margin-trim:inline))) "
    "or ((-moz-orient:inline) and (not (color:rgb(from red r g "
    "b)))){*,:before,:after,::backdrop{--tw-border-style:solid;--tw-font-weight:initial;--tw-blur:initial;--tw-"
    "brightness:initial;--tw-contrast:initial;--tw-grayscale:initial;--tw-hue-rotate:initial;--tw-invert:initial;--tw-"
    "opacity:initial;--tw-saturate:initial;--tw-sepia:initial;--tw-drop-shadow:initial;--tw-drop-shadow-color:initial;-"
    "-tw-drop-shadow-alpha:100%;--tw-drop-shadow-size:initial}}}@layer "
    "theme{:root,:host{--font-sans:ui-sans-serif,system-ui,sans-serif,\"Apple Color Emoji\",\"Segoe UI Emoji\",\"Segoe "
    "UI Symbol\",\"Noto Color Emoji\";--font-mono:ui-monospace,SFMono-Regular,Menlo,Monaco,Consolas,\"Liberation "
    "Mono\",\"Courier New\",monospace;--color-purple-200:oklch(90.2% .063 306.703);--color-zinc-50:oklch(98.5% 0 "
    "0);--color-zinc-100:oklch(96.7% .001 286.375);--color-zinc-200:oklch(92% .004 "
    "286.32);--color-zinc-300:oklch(87.1% .006 286.286);--color-zinc-500:oklch(55.2% .016 "
    "285.938);--color-white:#fff;--spacing:.25rem;--container-2xl:42rem;--container-3xl:48rem;--container-5xl:64rem;--"
    "container-7xl:80rem;--text-sm:.875rem;--text-sm--line-height:calc(1.25/"
    ".875);--text-xl:1.25rem;--text-xl--line-height:calc(1.75/1.25);--text-2xl:1.5rem;--text-2xl--line-height:calc(2/"
    "1.5);--font-weight-medium:500;--font-weight-bold:700;--radius-lg:.5rem;--radius-xl:.75rem;--default-transition-"
    "duration:.15s;--default-transition-timing-function:cubic-bezier(.4,0,.2,1);--default-font-family:var(--font-sans);"
    "--default-mono-font-family:var(--font-mono)}}@layer "
    "base{*,:after,:before,::backdrop{box-sizing:border-box;border:0 "
    "solid;margin:0;padding:0}::file-selector-button{box-sizing:border-box;border:0 "
    "solid;margin:0;padding:0}html,:host{-webkit-text-size-adjust:100%;-moz-tab-size:4;tab-size:4;line-height:1.5;font-"
    "family:var(--default-font-family,ui-sans-serif,system-ui,sans-serif,\"Apple Color Emoji\",\"Segoe UI "
    "Emoji\",\"Segoe UI Symbol\",\"Noto Color "
    "Emoji\");font-feature-settings:var(--default-font-feature-settings,normal);font-variation-settings:var(--default-"
    "font-variation-settings,normal);-webkit-tap-highlight-color:transparent}hr{height:0;color:inherit;border-top-"
    "width:1px}abbr:where([title]){-webkit-text-decoration:underline dotted;text-decoration:underline "
    "dotted}h1,h2,h3,h4,h5,h6{font-size:inherit;font-weight:inherit}a{color:inherit;-webkit-text-decoration:inherit;"
    "text-decoration:inherit}b,strong{font-weight:bolder}code,kbd,samp,pre{font-family:var(--default-mono-font-family,"
    "ui-monospace,SFMono-Regular,Menlo,Monaco,Consolas,\"Liberation Mono\",\"Courier "
    "New\",monospace);font-feature-settings:var(--default-mono-font-feature-settings,normal);font-variation-settings:"
    "var(--default-mono-font-variation-settings,normal);font-size:1em}small{font-size:80%}sub,sup{vertical-align:"
    "baseline;font-size:75%;line-height:0;position:relative}sub{bottom:-.25em}sup{top:-.5em}table{text-indent:0;border-"
    "color:inherit;border-collapse:collapse}:-moz-focusring{outline:auto}progress{vertical-align:baseline}summary{"
    "display:list-item}ol,ul,menu{list-style:none}img,svg,video,canvas,audio,iframe,embed,object{vertical-align:middle;"
    "display:block}img,video{max-width:100%;height:auto}button,input,select,optgroup,textarea{font:inherit;font-"
    "feature-settings:inherit;font-variation-settings:inherit;letter-spacing:inherit;color:inherit;opacity:1;"
    "background-color:#0000;border-radius:0}::file-selector-button{font:inherit;font-feature-settings:inherit;font-"
    "variation-settings:inherit;letter-spacing:inherit;color:inherit;opacity:1;background-color:#0000;border-radius:0}:"
    "where(select:is([multiple],[size])) optgroup{font-weight:bolder}:where(select:is([multiple],[size])) optgroup "
    "option{padding-inline-start:20px}::file-selector-button{margin-inline-end:4px}::placeholder{opacity:1}@supports "
    "(not ((-webkit-appearance:-apple-pay-button))) or "
    "(contain-intrinsic-size:1px){::placeholder{color:currentColor}@supports (color:color-mix(in "
    "lab,red,red)){::placeholder{color:color-mix(in oklab,currentcolor "
    "50%,transparent)}}}textarea{resize:vertical}::-webkit-search-decoration{-webkit-appearance:none}::-webkit-date-"
    "and-time-value{min-height:1lh;text-align:inherit}::-webkit-datetime-edit{display:inline-flex}::-webkit-datetime-"
    "edit-fields-wrapper{padding:0}::-webkit-datetime-edit{padding-block:0}::-webkit-datetime-edit-year-field{padding-"
    "block:0}::-webkit-datetime-edit-month-field{padding-block:0}::-webkit-datetime-edit-day-field{padding-block:0}::-"
    "webkit-datetime-edit-hour-field{padding-block:0}::-webkit-datetime-edit-minute-field{padding-block:0}::-webkit-"
    "datetime-edit-second-field{padding-block:0}::-webkit-datetime-edit-millisecond-field{padding-block:0}::-webkit-"
    "datetime-edit-meridiem-field{padding-block:0}:-moz-ui-invalid{box-shadow:none}button,input:where([type=button],["
    "type=reset],[type=submit]){-webkit-appearance:button;-moz-appearance:button;appearance:button}::file-selector-"
    "button{-webkit-appearance:button;-moz-appearance:button;appearance:button}::-webkit-inner-spin-button{height:auto}"
    "::-webkit-outer-spin-button{height:auto}[hidden]:where(:not([hidden=until-found])){display:none!important}}@layer "
    "components;@layer "
    "utilities{.fixed{position:fixed}.static{position:static}.sticky{position:sticky}.isolate{isolation:isolate}."
    "container{width:100%}@media (min-width:40rem){.container{max-width:40rem}}@media "
    "(min-width:48rem){.container{max-width:48rem}}@media (min-width:64rem){.container{max-width:64rem}}@media "
    "(min-width:80rem){.container{max-width:80rem}}@media "
    "(min-width:96rem){.container{max-width:96rem}}.mx-auto{margin-inline:auto}.mb-1{margin-bottom:calc(var(--spacing)*"
    "1)}.block{display:block}.contents{display:contents}.flex{display:flex}.grid{display:grid}.inline{display:inline}."
    "inline-block{display:inline-block}.table{display:table}.size-12{width:calc(var(--spacing)*12);height:calc(var(--"
    "spacing)*12)}.min-h-screen{min-height:100vh}.w-full{width:100%}.min-w-\\[320px\\]{min-width:320px}.flex-auto{flex:"
    "auto}.grow{flex-grow:1}.grid-cols-1{grid-template-columns:repeat(1,minmax(0,1fr))}.flex-col{flex-direction:column}"
    ".items-center{align-items:center}.justify-between{justify-content:space-between}.gap-2{gap:calc(var(--spacing)*2)}"
    ".gap-4{gap:calc(var(--spacing)*4)}.rounded-lg{border-radius:var(--radius-lg)}.border{border-style:var(--tw-border-"
    "style);border-width:1px}.border-y-8{border-block-style:var(--tw-border-style);border-block-width:8px}.border-zinc-"
    "200{border-color:var(--color-zinc-200)}.border-zinc-200\\/60{border-color:#e4e4e799}@supports (color:color-mix(in "
    "lab,red,red)){.border-zinc-200\\/60{border-color:color-mix(in "
    "oklab,var(--color-zinc-200)60%,transparent)}}.bg-white{background-color:var(--color-white)}.bg-zinc-100{"
    "background-color:var(--color-zinc-100)}.p-4{padding:calc(var(--spacing)*4)}.p-5{padding:calc(var(--spacing)*5)}."
    "px-4{padding-inline:calc(var(--spacing)*4)}.pt-6{padding-top:calc(var(--spacing)*6)}.text-center{text-align:"
    "center}.text-2xl{font-size:var(--text-2xl);line-height:var(--tw-leading,var(--text-2xl--line-height))}.text-sm{"
    "font-size:var(--text-sm);line-height:var(--tw-leading,var(--text-sm--line-height))}.text-xl{font-size:var(--text-"
    "xl);line-height:var(--tw-leading,var(--text-xl--line-height))}.font-bold{--tw-font-weight:var(--font-weight-bold);"
    "font-weight:var(--font-weight-bold)}.font-medium{--tw-font-weight:var(--font-weight-medium);font-weight:var(--"
    "font-weight-medium)}.text-zinc-300{color:var(--color-zinc-300)}.text-zinc-500{color:var(--color-zinc-500)}.filter{"
    "filter:var(--tw-blur,)var(--tw-brightness,)var(--tw-contrast,)var(--tw-grayscale,)var(--tw-hue-rotate,)var(--tw-"
    "invert,)var(--tw-saturate,)var(--tw-sepia,)var(--tw-drop-shadow,)}.transition{transition-property:color,"
    "background-color,border-color,outline-color,text-decoration-color,fill,stroke,--tw-gradient-from,--tw-gradient-"
    "via,--tw-gradient-to,opacity,box-shadow,transform,translate,scale,rotate,filter,-webkit-backdrop-filter,backdrop-"
    "filter,display,visibility,content-visibility,overlay,pointer-events;transition-timing-function:var(--tw-ease,var(-"
    "-default-transition-timing-function));transition-duration:var(--tw-duration,var(--default-transition-duration))}."
    "\\[env\\:nucleo_f446re\\]{env:nucleo f446re}@media "
    "(hover:hover){.hover\\:bg-zinc-50\\/50:hover{background-color:#fafafa80}@supports (color:color-mix(in "
    "lab,red,red)){.hover\\:bg-zinc-50\\/50:hover{background-color:color-mix(in "
    "oklab,var(--color-zinc-50)50%,transparent)}}}.active\\:border-purple-200:active{border-color:var(--color-purple-"
    "200)}@media "
    "(min-width:40rem){.sm\\:max-w-2xl{max-width:var(--container-2xl)}.sm\\:grid-cols-2{grid-template-columns:repeat(2,"
    "minmax(0,1fr))}.sm\\:flex-row{flex-direction:row}.sm\\:items-center{align-items:center}.sm\\:justify-between{"
    "justify-content:space-between}.sm\\:rounded-xl{border-radius:var(--radius-xl)}.sm\\:border-8{border-style:var(--"
    "tw-border-style);border-width:8px}.sm\\:text-start{text-align:start}}@media "
    "(min-width:48rem){.md\\:max-w-3xl{max-width:var(--container-3xl)}}@media "
    "(min-width:64rem){.lg\\:col-span-3{grid-column:span 3/span "
    "3}.lg\\:max-w-5xl{max-width:var(--container-5xl)}.lg\\:grid-cols-12{grid-template-columns:repeat(12,minmax(0,1fr))"
    "}.lg\\:gap-8{gap:calc(var(--spacing)*8)}.lg\\:p-8{padding:calc(var(--spacing)*8)}.lg\\:px-8{padding-inline:calc("
    "var(--spacing)*8)}.lg\\:pt-8{padding-top:calc(var(--spacing)*8)}}@media "
    "(min-width:80rem){.xl\\:max-w-7xl{max-width:var(--container-7xl)}}}@property "
    "--tw-border-style{syntax:\"*\";inherits:false;initial-value:solid}@property "
    "--tw-font-weight{syntax:\"*\";inherits:false}@property --tw-blur{syntax:\"*\";inherits:false}@property "
    "--tw-brightness{syntax:\"*\";inherits:false}@property --tw-contrast{syntax:\"*\";inherits:false}@property "
    "--tw-grayscale{syntax:\"*\";inherits:false}@property --tw-hue-rotate{syntax:\"*\";inherits:false}@property "
    "--tw-invert{syntax:\"*\";inherits:false}@property --tw-opacity{syntax:\"*\";inherits:false}@property "
    "--tw-saturate{syntax:\"*\";inherits:false}@property --tw-sepia{syntax:\"*\";inherits:false}@property "
    "--tw-drop-shadow{syntax:\"*\";inherits:false}@property "
    "--tw-drop-shadow-color{syntax:\"*\";inherits:false}@property "
    "--tw-drop-shadow-alpha{syntax:\"<percentage>\";inherits:false;initial-value:100%}@property "
    "--tw-drop-shadow-size{syntax:\"*\";inherits:false}</style></head><body><div class=\"mx-auto flex min-h-screen "
    "w-full min-w-[320px] flex-col bg-zinc-100\" ><main class=\"mx-auto flex w-full flex-auto flex-col border-y-8 "
    "border-zinc-200/60 bg-white sm:max-w-2xl sm:rounded-xl sm:border-8 md:max-w-3xl lg:max-w-5xl xl:max-w-7xl\" ><div "
    "class=\"container mx-auto px-4 pt-6 lg:px-8 lg:pt-8 gap-2 text-center sm:flex-row sm:items-center "
    "sm:justify-between sm:text-start\" ><h1 class=\"mb-1 text-xl font-bold\">Контроль мікроклімату</h1><h2 "
    "class=\"text-sm font-medium text-zinc-500\"> Ласкаво просимо, у Вас є доступ до <strong>3 датчиків</strong>. "
    "</h2></div><div class=\"container mx-auto p-4 lg:p-8 xl:max-w-7xl grid grid-cols-1 gap-4 sm:grid-cols-2 "
    "lg:grid-cols-12 lg:gap-8\" ><div class=\"rounded-lg border border-zinc-200 bg-white hover:bg-zinc-50/50 "
    "active:border-purple-200 lg:col-span-3 flex grow items-center justify-between p-5\" ><dl><dt class=\"text-2xl "
    "font-bold\">3°C</dt><dd class=\"text-sm font-medium text-zinc-500\">Температура</dd></dl><div class=\"flex "
    "items-center text-sm font-medium text-zinc-300\"><svg xmlns=\"http://www.w3.org/2000/svg\" fill=\"none\" "
    "viewBox=\"0 0 24 24\" stroke-width=\"1.5\" stroke=\"currentColor\" data-slot=\"icon\" class=\"hi-outline "
    "hi-server inline-block size-12\" ><path d=\"M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z\" "
    "></path></svg></div></div><div class=\"rounded-lg border border-zinc-200 bg-white hover:bg-zinc-50/50 "
    "active:border-purple-200 lg:col-span-3 flex grow items-center justify-between p-5\" ><dl><dt class=\"text-2xl "
    "font-bold\">10%</dt><dd class=\"text-sm font-medium text-zinc-500\">Вологість</dd></dl><div class=\"flex "
    "items-center text-sm font-medium text-zinc-300\"><svg xmlns=\"http://www.w3.org/2000/svg\" fill=\"none\" "
    "viewBox=\"0 0 24 24\" stroke-width=\"1.5\" stroke=\"currentColor\" data-slot=\"icon\" class=\"hi-outline "
    "hi-globe-alt inline-block size-12\" ><path d=\"M12 2.69l5.66 5.66a8 8 0 1 1-11.31 "
    "0z\"></path></svg></div></div><div class=\"rounded-lg border border-zinc-200 bg-white hover:bg-zinc-50/50 "
    "active:border-purple-200 lg:col-span-3 flex grow items-center justify-between p-5\" ><dl><dt class=\"text-2xl "
    "font-bold\">24/4095</dt><dd class=\"text-sm font-medium text-zinc-500\">Освітленість</dd></dl><div class=\"flex "
    "items-center text-sm font-medium text-zinc-300\"><svg xmlns=\"http://www.w3.org/2000/svg\" fill=\"none\" "
    "viewBox=\"0 0 24 24\" stroke-width=\"1.5\" stroke=\"currentColor\" class=\"size-12\" ><path "
    "stroke-linecap=\"round\" stroke-linejoin=\"round\" d=\"M12 3v2.25m6.364.386-1.591 1.591M21 12h-2.25m-.386 "
    "6.364-1.591-1.591M12 18.75V21m-4.773-4.227-1.591 1.591M5.25 12H3m4.227-4.773L5.636 5.636M15.75 12a3.75 3.75 0 1 "
    "1-7.5 0 3.75 3.75 0 0 1 7.5 0Z\" /></svg></div></div></div></main></div></body></html>";

static inline void getData(char *data) {
  const uint16_t timeout = 1000;
  HAL_UART_Receive(&huart1, (uint8_t *)data, GET_DATA_MAX_LENGTH, timeout);
}

static inline void sendData(const char *data) {
  const uint16_t timeout = 1000;
  HAL_UART_Transmit(&huart1, (uint8_t *)data, strlen(data), timeout);
}

static inline void sendCommandUntilInData(const char *command, uint16_t delay, const char *substring) {
  while (1) {
    char response[GET_DATA_MAX_LENGTH] = {0};
    sendData(command);
    getData(response);
    if (strstr(response, substring)) {
      break;
    }
    HAL_Delay(delay);
  }
}

void initialiseEsp8266() {
  const char *commands[] = {"AT+RST\r\n",
                            "AT+CWMODE_CUR=2\r\n",
                            "AT+CWSAP_CUR=\"STM32\",\"12345678\",1,3,4,0\r\n",
                            "AT+CIPAP_CUR=\"192.168.51.1\"\r\n",
                            "AT+CIPMUX=1\r\n",
                            "AT+CIPSERVER=1,80\r\n"};
  const uint16_t delay = 500;
  for (uint32_t i = 0; i < lengthof(commands); ++i) {
    sendCommandUntilInData(commands[i], delay, "OK");
  }
}

static inline int8_t getChannelNumber(const char *data) {
  const uint8_t channelNumber = 8;
  char substring[SUBSTRING_LENGTH] = {0};
  for (int8_t i = 0; i < channelNumber; ++i) {
    (void)sprintf(substring, "+IPD,%d", i);
    if (strstr(data, substring)) {
      return i;
    }
  }
  return -1;
}

static inline void sendHtmlChunk(const char *chunk, uint8_t channelNumber) {
  char command[COMMAND_LENGTH] = {0};
  (void)sprintf(command, "AT+CIPSEND=%d,%d\r\n", channelNumber, strlen(chunk));
  sendCommandUntilInData(command, 1, ">");
  HAL_Delay(1);
  sendData(chunk);
  const uint8_t delay = 100;
  HAL_Delay(delay);
}

void runEsp8266() {
  char data[GET_DATA_MAX_LENGTH] = {0};
  getData(data);
  int8_t channelNumber = getChannelNumber(data);
  if (channelNumber == -1) {
    return;
  }
  uint16_t htmlLength = strlen(html);
  char htmlChunk[SEND_DATA_MAX_LENGTH + 1] = {0};
  for (size_t i = 0; i < htmlLength; i += SEND_DATA_MAX_LENGTH) {
    uint16_t currentChunkSize = htmlLength >= SEND_DATA_MAX_LENGTH + i ? SEND_DATA_MAX_LENGTH : htmlLength - i;
    strncpy(htmlChunk, html + i, currentChunkSize);
    htmlChunk[currentChunkSize] = '\0';
    sendHtmlChunk(htmlChunk, channelNumber);
  }
  char command[COMMAND_LENGTH] = {0};
  (void)sprintf(command, "AT+CIPCLOSE=%d\r\n", channelNumber);
  const uint16_t delay = 500;
  sendCommandUntilInData(command, delay, "OK");
}
