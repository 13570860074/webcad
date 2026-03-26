import { createApp } from 'vue'
import App from './App.vue'
import './style.css'
import { application } from './Application/FApplication'
import { gcadGlobal, gcadGlobal_updateUiLayoutData } from './gcadGlobal'





/** 设置默认尺寸 */
gcadGlobal.uiLayoutData = {
    
    /** 设置工作区(相对于dody的尺寸) */
    workLeft: 0,
    workTop: 0,
    workRight:0 + document.documentElement.clientWidth - 0,
    workBottom: 0 + document.documentElement.clientHeight -0,
    
    /**菜单栏高度 */
    menubarHeight: 48,

    /**工具栏高度 */
    toolbarHeight: 90,
};
gcadGlobal_updateUiLayoutData(gcadGlobal.uiLayoutData, false);


// 加载应用
application.load();



const app = createApp(App)
app.mount('#app')

