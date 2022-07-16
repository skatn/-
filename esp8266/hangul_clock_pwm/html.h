#ifndef HTML_H
#define HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Hangul Clock</title>

    <script src="https://cdnjs.cloudflare.com/ajax/libs/axios/0.19.0/axios.min.js"></script>
</head>
<body>
    <h1>한글시계 설정페이지</h1>
    <button onclick="onClickGetSettings();">
        세팅값 가져오기
    </button>

    <p>디스플레이 모드</p>
    <span>현재 설정 모드 : </span><span id="display_mode">기본</span>
    <br>
    <input type="radio" name="display_mode" value="0">기본
    <input type="radio" name="display_mode" value="1">페이드
    <button onclick="onClickSetDisplayMode();">설정</button>
    <hr/>

    <p>밝기</p>
    <span>현재 설정 값 : </span><span id="brightness">50%</span>
    <br>
    <input type="range" id="slider" value="0" min="0" max="100" onchange="changeSlideValue(this.value)">
    <span id="slide_value">0%</span>
    <button onclick="onClickSetBrightness();">설정</button>
    <hr/>

    <script language = "javascript">
        const brightness = document.getElementById("brightness");
        const slider = document.getElementById("slider");
        const slideValue = document.getElementById('slide_value');
        const displayMode = document.getElementById('display_mode');
        const displayRadios = document.getElementsByName("display_mode")

        function onClickGetSettings(){
            axios.post("http://localhost/get_settings", {})
            .then(function(result){
                console.log(result);
                const split = result.split(",");
                const display_mode = split[0];
                const brightness = split[1];

                updateDisplay(display_mode);
                updateBrightness(brightness);
            })
            .catch(function(error){
                console.log(error);
            })
        }

        function onClickSetDisplayMode(){
            let displayMode = 0;
            for(let i=0; i<displayRadios.length; i++){
                if(displayRadios[i].checked === true){
                    displayMode = displayRadios[i].value;
                    break;
                }
            }
            axios.post("http://localhost/set_display_mode", {
                mode: displayMode
            })
            .then(function(result){
                console.log(result);
                if(result==="SUCCESS"){
                    updateDisplay(Number(displayMode));
                }
            })
            .catch(function(error){
                console.log(error);
            });
        }

        function onClickSetBrightness(){
            const value = slider.value;
            axios.post("http://localhost/set_brightness", {
                brightness: value
            })
            .then(function(result){
                console.log(result);
                if(result === "SUCCESS"){
                    updateBrightness(Number(result));
                }
            })
            .catch(function(error){
                console.log(error);
            })
        }


        function updateDisplay(mode){
            if(mode === 0){
                displayRadios[0].checked = true;
                displayMode.innerText = "기본";
            }
            else{
                displayRadios[1].checked = true;
                displayMode.innerText = "페이드";
            }
        }

        function updateBrightness(value){
            slider.value = value;
            brightness.innerText = value;
        }

        function changeSlideValue(value){
            slideValue.innerText = value.toString() + "%";
        }
    </script>
</body>
</html>
)rawliteral";

#endif 
