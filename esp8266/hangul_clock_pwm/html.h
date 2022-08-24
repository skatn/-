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
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">
</head>
<body class="bg-light">
    <div class="container-md">
        <h1 class="display-4">한글시계 설정페이지</h1>
    
        <section class="shadow-sm p-3 mb-3 bg-body rounded">
            <h4>디스플레이 모드</h4>
            <span>현재 설정 모드 : </span><span id="display_mode">-</span>
            <br>
            <div class="form-check form-check-inline">
                <input class="form-check-input" type="radio" name="display_mode" value="0" id="display_standard">
                <label class="form-check-label" for="display_standard" >기본</label>
            </div>
            <div class="form-check form-check-inline">
                <input class="form-check-input" type="radio" name="display_mode" value="1" id="display_fade">
                <label class="form-check-label" for="display_fade">페이드
            </div>
            <button class="btn btn-primary" onclick="onClickSetDisplayMode();">
                적용
            </button>
        </section>
    
        <section class="shadow-sm p-3 mb-3 bg-body rounded">
            <h4>밝기</h4>
            <span>현재 설정 값 : </span><span id="brightness">-%</span>
            <br>
            <input type="range" id="slider" value="0" min="0" max="100" onchange="changeSlideValue(this.value)">
            <span id="slide_value">0%</span>
            <button class="btn btn-primary" onclick="onClickSetBrightness();">
                적용
            </button>
        </section>
        
        <button class="btn btn-primary" onclick="onClickGetSettings();">
            세팅값 가져오기
        </button>
    </div>

    <script language = "javascript">
        const brightness = document.getElementById("brightness");
        const slider = document.getElementById("slider");
        const slideValue = document.getElementById('slide_value');
        const displayMode = document.getElementById('display_mode');
        const displayRadios = document.getElementsByName("display_mode")

        const server = `http://${window.location.host}`;

        function onClickGetSettings(){
            request(`${server}/get_settings`)
            .then((result)=>{
                console.log(result);
                const split = result.split(",");
                const display_mode = split[0];
                const brightness = split[1];

                updateDisplay(Number(display_mode));
                updateBrightness(brightness);
            })
            .catch((error)=>{
                console.log(error);
                alert("세팅값 가져오기에 실패하였습니다.");
            });
        }

        function onClickSetDisplayMode(){
            let displayMode;
            for(let i=0; i<displayRadios.length; i++){
                if(displayRadios[i].checked === true){
                    displayMode = displayRadios[i].value;
                    break;
                }
            }

            if(displayMode === undefined){
                alert("모드를 선택해 주세요");
                return;
            }

            request(`${server}/set_display_mode?mode=${displayMode}`)
            .then((result)=>{
                if(result==="SUCCESS"){
                    updateDisplay(Number(displayMode));
                }
            })
            .catch((error)=>{
                console.log(error);
                alert("디스플레이 모드 설정에 실패하였습니다.");
            });
        }

        function onClickSetBrightness(){
            const value = slider.value;
            request(`${server}/set_brightness?brightness=${value}`)
            .then((result)=>{
                if(result === "SUCCESS"){
                    updateBrightness(value);
                }
            })
            .catch((error)=>{
                console.log(error);
                alert("밝기 설정에 실패하였습니다.");
            });
        }

        function loadingStart(){
            const buttons = document.getElementsByTagName("button");
            for(const button of buttons){
                button.disabled = true;
                const spinner = document.createElement("span");
                spinner.setAttribute("class", "spinner-border spinner-border-sm");
                spinner.setAttribute("role", "status");
                spinner.setAttribute("aria-hidden", "true");

                button.appendChild(spinner);
            }
        }

        function loadingEnd(){
            const buttons = document.getElementsByTagName("button");
            for(const button of buttons){
                button.disabled = false;
                const spinner = button.childNodes[1]
                button.removeChild(spinner);
            }
        }

        function request(uri){
            loadingStart();
            return new Promise((resolve, reject)=>{
                var xhttp = new XMLHttpRequest();
                
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        console.log(xhttp.responseText);
                        resolve(xhttp.responseText);
                        loadingEnd();
                    }
                };
                xhttp.onerror = function () {
                    loadingEnd();
                    reject("ERROR");
                };
        
                xhttp.open("GET", uri, true);
                xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
                xhttp.send();
            });
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
            slideValue.innerText = value.toString() + "%";
        }

        function changeSlideValue(value){
            slideValue.innerText = value.toString() + "%";
        }
    </script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p" crossorigin="anonymous"></script>
</body>
</html>
)rawliteral";

#endif 
