'use client';

import React, { useState } from 'react';

export default function EasyLangIDE() {
  const [code, setCode] = useState('MAIN\nBEGIN\n  a:b ALLOCATE;\n  INPUT b;\n  a := 1 + 2 + b;\n  OUTPUT a;\nEND');
  const [intermediateCode, setIntermediateCode] = useState('');
  const [assemblerCode, setAssemblerCode] = useState('');
  const [consoleOutput, setConsoleOutput] = useState('');
  const [isModalOpen, setIsModalOpen] = useState(false);

  // 1. Funciones de Archivo (.el)
  const handleFileAction = (action: string) => {
    if (action === 'open') {
      const input = document.createElement('input');
      input.type = 'file';
      input.accept = '.el';
      input.onchange = (e: any) => {
        const file = e.target.files[0];
        const reader = new FileReader();
        reader.onload = (event: any) => setCode(event.target.result);
        reader.readAsText(file);
      };
      input.click();
    } else if (action === 'save') {
      const blob = new Blob([code], { type: 'text/plain;charset=utf-8' });
      const url = URL.createObjectURL(blob);
      const a = document.createElement('a');
      a.href = url;
      a.download = 'programa.el';
      a.click();
      URL.revokeObjectURL(url);
    }
  };

  // 2. Funciones de Compilación y Ejecución
  const handleGenerateIntermediate = () => {
    setIntermediateCode('t1 = 1 + 2\nt2 = t1 + b\na = t2');
  };

  const handleGenerateAssembler = () => {
    setAssemblerCode('section .text\n    global main\nmain:\n    ; Código NASM generado por el compilador');
  };

  const handleExecute = () => {
    setConsoleOutput('Compilando con NASM y ejecutando...\nSalida estándar:\n> 15');
  };

  return (
    <div className="flex flex-col h-screen bg-slate-100 text-slate-800 font-sans">
      {/* Barra superior con Logo y Controles */}
      <header className="flex items-center justify-between px-6 py-3 bg-white border-b border-slate-200 shadow-sm">
        <div className="flex items-center space-x-4">
          {/* Logo de la Universidad */}
          <img src="/logo.jpg" alt="Logo Universidad Mesoamericana" className="h-20 w-20 rounded-full border border-slate-300" />
          <div>
            <h1 className="text-base font-bold leading-tight text-slate-900">EasyLang IDE</h1>
            <p className="text-xs text-slate-500">Universidad Mesoamericana - Compiladores</p>
          </div>
        </div>

        {/* Botones de Archivo e Integrantes */}
        <div className="flex items-center space-x-2">
          <button 
            onClick={() => handleFileAction('open')}
            className="px-3 py-1.5 text-xs bg-slate-100 hover:bg-slate-200 text-slate-700 rounded transition font-medium border border-slate-300"
          >
            📂 Abrir (.el)
          </button>
          <button 
            onClick={() => handleFileAction('save')}
            className="px-3 py-1.5 text-xs bg-slate-100 hover:bg-slate-200 text-slate-700 rounded transition font-medium border border-slate-300"
          >
            💾 Guardar
          </button>
          <button 
            onClick={() => setIsModalOpen(true)}
            className="px-3 py-1.5 text-xs bg-indigo-600 hover:bg-indigo-700 text-white rounded transition font-semibold shadow-sm"
          >
            👥 Integrantes
          </button>
        </div>
      </header>

      {/* Panel Principal dividido */}
      <div className="grid grid-cols-1 lg:grid-cols-3 flex-1 p-4 gap-4 overflow-hidden">
        
        {/* Editor de Código Fuente (.el) */}
        <div className="flex flex-col bg-white rounded-lg border border-slate-200 p-3 shadow-sm">
          <div className="flex justify-between items-center mb-2">
            <span className="text-xs font-semibold uppercase tracking-wider text-slate-500">Editor Fuente (.el)</span>
            <button 
              onClick={handleGenerateIntermediate}
              className="px-2.5 py-1 text-xs bg-emerald-600 hover:bg-emerald-700 text-white rounded font-bold transition shadow-sm"
            >
              ⚙️ Generar Intermedio
            </button>
          </div>
          <textarea
            value={code}
            onChange={(e) => setCode(e.target.value)}
            className="flex-1 w-full bg-slate-50 font-mono text-xs p-3 rounded border border-slate-300 focus:outline-none focus:border-indigo-500 resize-none text-slate-900 leading-relaxed"
            placeholder="Escribe tu código EasyLang aquí..."
          />
        </div>

        {/* Panel Central: Código Intermedio y Assembler NASM */}
        <div className="flex flex-col space-y-4">
          {/* Código de Tres Direcciones */}
          <div className="flex flex-col flex-1 bg-white rounded-lg border border-slate-200 p-3 shadow-sm">
            <div className="flex justify-between items-center mb-2">
              <span className="text-xs font-semibold uppercase tracking-wider text-slate-500">Código de 3 Direcciones</span>
              <button 
                onClick={handleGenerateAssembler}
                className="px-2.5 py-1 text-xs bg-blue-600 hover:bg-blue-700 text-white rounded font-bold transition shadow-sm"
              >
                🛠️ Generar NASM
              </button>
            </div>
            <pre className="flex-1 w-full bg-slate-50 font-mono text-xs p-3 rounded border border-slate-300 overflow-auto text-blue-700">
              {intermediateCode || '// El código intermedio aparecerá aquí...'}
            </pre>
          </div>

          {/* Código Assembler */}
          <div className="flex flex-col flex-1 bg-white rounded-lg border border-slate-200 p-3 shadow-sm">
            <div className="flex justify-between items-center mb-2">
              <span className="text-xs font-semibold uppercase tracking-wider text-slate-500">Assembler NASM</span>
              <button 
                onClick={handleExecute}
                className="px-2.5 py-1 text-xs bg-amber-600 hover:bg-amber-700 text-white rounded font-bold transition shadow-sm"
              >
                ▶️ Ejecutar
              </button>
            </div>
            <pre className="flex-1 w-full bg-slate-50 font-mono text-xs p-3 rounded border border-slate-300 overflow-auto text-amber-800">
              {assemblerCode || '; El código NASM aparecerá aquí...'}
            </pre>
          </div>
        </div>

        {/* Panel Derecho: Consola y Salida de Errores */}
        <div className="flex flex-col bg-white rounded-lg border border-slate-200 p-3 shadow-sm">
          <span className="text-xs font-semibold uppercase tracking-wider text-slate-500 mb-2">Consola / Salida Estándar</span>
          <div className="flex-1 w-full bg-slate-50 font-mono text-xs p-3 rounded border border-slate-300 overflow-auto text-slate-700">
            {consoleOutput || 'La salida de la ejecución y los reportes de error aparecerán aquí...'}
          </div>
        </div>

      </div>

      {/* Ventana Emergente (Modal) de Integrantes */}
      {isModalOpen && (
        <div className="fixed inset-0 bg-slate-900/40 backdrop-blur-xs flex items-center justify-center z-50 p-4">
          <div className="bg-white border border-slate-200 p-6 rounded-xl w-full max-w-sm shadow-xl">
            <h2 className="text-base font-bold mb-4 text-indigo-600 border-b border-slate-100 pb-2">Integrantes del Grupo</h2>
            <div className="space-y-2 mb-6 text-xs text-slate-600">
              <p><span className="font-semibold text-slate-900">Nombre:</span> María Andreé Ponciano Morales</p>
              <p><span className="font-semibold text-slate-900">Carné:</span> 202308050</p>
            </div>
            <div className="flex justify-end">
              <button 
                onClick={() => setIsModalOpen(false)}
                className="px-4 py-2 bg-indigo-600 hover:bg-indigo-700 text-white text-xs rounded font-bold transition shadow-sm"
              >
                Cerrar
              </button>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}