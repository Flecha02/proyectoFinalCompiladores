'use client';

import React, { useState } from 'react';

export default function EasyLangIDE() {
  const [code, setCode] = useState('');
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

  // 2. Función unificada para consumir la API del Backend (Node.js + Compilador en C)
  const handleCompileBackend = async () => {
    try {
      setConsoleOutput('Enviando código al servidor de compilación...');
      const respuesta = await fetch('http://localhost:5000/api/compile', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ codigo: code })
      });
      const resultado = await respuesta.json();
      
      // Asignamos únicamente lo que retorna la API del backend
      setIntermediateCode(resultado.tac || '// Sin código TAC generado');
      setAssemblerCode(resultado.asm || '; Sin código NASM generado');
      setConsoleOutput(resultado.mensaje || 'Proceso completado.');
      
      return resultado;
    } catch (error) {
      setConsoleOutput('Error de conexión con el backend en Node.js:\n' + error);
      return null;
    }
  };

  const handleGenerateIntermediate = async () => {
    await handleCompileBackend();
  };

  const handleGenerateAssembler = async () => {
    await handleCompileBackend();
  };

  const handleExecute = async () => {
    const resultado = await handleCompileBackend();
    if (resultado && resultado.exito) {
      setConsoleOutput(
        '=== COMPILACIÓN Y TRADUCCIÓN EXITOSA ===\n' +
        '1. Archivo codigo.tac generado en el servidor.\n' +
        '2. Archivo codigo.asm generado con éxito en el servidor.\n\n' +
        'Puedes ensamblar y correr en tu Ubuntu con:\n' +
        '  nasm -f elf32 codigo.asm -o codigo.o\n' +
        '  gcc -m32 codigo.o -o programa\n' +
        '  ./programa'
      );
    }
  };

  return (
    <div className="flex flex-col h-screen bg-slate-100 text-slate-800 font-sans overflow-hidden">
      {/* Barra superior con Logo y Controles */}
      <header className="flex items-center justify-between px-6 py-3 bg-white border-b border-slate-200 shadow-sm shrink-0">
        <div className="flex items-center space-x-4">
          {/* Logo de la Universidad */}
          <img src="/logo.jpg" alt="Logo Universidad" className="h-20 w-auto" />
          <div>
            <h1 className="text-base font-bold leading-tight text-slate-900">COMPILADORES</h1>
          </div>
        </div>

        {/* Botones de Archivo e Integrantes (Sin botón de Guardar) */}
        <div className="flex items-center space-x-2">
          <button 
            onClick={handleFileAction.bind(null, 'open')}
            className="px-3 py-1.5 text-xs bg-slate-100 hover:bg-slate-200 text-slate-700 rounded transition font-medium border border-slate-300"
          >
            Abrir (.el)
          </button>
          <button 
            onClick={() => setIsModalOpen(true)}
            className="px-3 py-1.5 text-xs bg-indigo-600 hover:bg-indigo-700 text-white rounded transition font-semibold shadow-sm"
          >
            Integrantes
          </button>
        </div>
      </header>

  
      <div className="grid grid-cols-1 lg:grid-cols-3 flex-1 p-4 gap-4 overflow-hidden">
        
        {/* Editor de Código Fuente (.el) */}
        <div className="flex flex-col bg-white rounded-lg border border-slate-200 p-3 shadow-sm overflow-hidden">
          <div className="flex justify-between items-center mb-2 shrink-0">
            <span className="text-xs font-semibold uppercase tracking-wider text-slate-500">Editor Fuente (.el)</span>
            <button 
              onClick={handleCompileBackend}
              className="px-3 py-1.5 text-xs bg-emerald-600 hover:bg-emerald-700 text-white rounded font-bold transition shadow-sm"
            >
              Generar Código
            </button>
          </div>
          <div className="flex-1 relative overflow-hidden rounded border border-slate-300 bg-slate-50">
            <textarea
              value={code}
              onChange={(e) => setCode(e.target.value)}
              className="absolute inset-0 w-full h-full bg-transparent font-mono text-xs p-3 focus:outline-none resize-none text-slate-900 leading-relaxed overflow-auto whitespace-pre"
              placeholder="Escribe tu código EasyLang aquí..."
            />
          </div>
        </div>

        {/* Panel Central: Código Intermedio y Assembler NASM */}
        <div className="flex flex-col space-y-4 overflow-hidden">
          {/* Código de Tres Direcciones */}
          <div className="flex flex-col flex-1 bg-white rounded-lg border border-slate-200 p-3 shadow-sm overflow-hidden">
            <div className="flex justify-between items-center mb-2 shrink-0">
              <span className="text-xs font-semibold uppercase tracking-wider text-slate-500">Código de 3 Direcciones</span>
            </div>
            <div className="flex-1 relative overflow-hidden rounded border border-slate-300 bg-slate-50">
              <pre className="absolute inset-0 w-full h-full font-mono text-xs p-3 overflow-auto text-blue-700 whitespace-pre">
                {intermediateCode || '// El código intermedio aparecerá aquí al generar...'}
              </pre>
            </div>
          </div>

          {/* Código Assembler (Generado automáticamente) */}
          <div className="flex flex-col flex-1 bg-white rounded-lg border border-slate-200 p-3 shadow-sm overflow-hidden">
            <div className="flex justify-between items-center mb-2 shrink-0">
              <span className="text-xs font-semibold uppercase tracking-wider text-slate-500">Assembler NASM</span>
              <button 
                onClick={handleExecute}
                className="px-2.5 py-1 text-xs bg-amber-600 hover:bg-amber-700 text-white rounded font-bold transition shadow-sm"
              >
                Ejecutar
              </button>
            </div>
            <div className="flex-1 relative overflow-hidden rounded border border-slate-300 bg-slate-50">
              <pre className="absolute inset-0 w-full h-full font-mono text-xs p-3 overflow-auto text-amber-800 whitespace-pre">
                {assemblerCode || '; El código NASM aparecerá aquí automáticamente...'}
              </pre>
            </div>
          </div>
        </div>

        {/* Panel Derecho: Consola y Salida de Errores */}
        <div className="flex flex-col bg-white rounded-lg border border-slate-200 p-3 shadow-sm overflow-hidden">
          <span className="text-xs font-semibold uppercase tracking-wider text-slate-500 mb-2 shrink-0">Consola / Salida Estándar</span>
          <div className="flex-1 relative overflow-hidden rounded border border-slate-300 bg-slate-50">
            <pre className="absolute inset-0 w-full h-full font-mono text-xs p-3 overflow-auto text-slate-700 whitespace-pre">
              {consoleOutput || 'La salida de la ejecución y los reportes de error aparecerán aquí...'}
            </pre>
          </div>
        </div>

      </div>

      {/* Ventana Emergente de Integrantes */}
      {isModalOpen && (
        <div className="fixed inset-0 bg-slate-900/40 backdrop-blur-xs flex items-center justify-center z-50 p-4">
          <div className="bg-white border border-slate-200 p-6 rounded-xl w-full max-w-sm shadow-xl">
            <h2 className="text-base font-bold mb-4 text-indigo-600 border-b border-slate-100 pb-2">Integrantes del Grupo</h2>
            <div className="space-y-2 mb-6 text-xs text-slate-600">
              <h1 className="text-sm font-semibold text-slate-900">Integrantes:</h1>
              <p className="font-semibold text-slate-900">Maria Andrée Ponciano Morales</p>
              <p className="font-semibold text-slate-900">Andres Javier Lopez de Leon</p>
              <p className="font-semibold text-slate-900">Carlos Rolando Sebastian Soch Ventura</p>
               <p className="font-semibold text-slate-900">Paulo Julian Lepe Calderon</p>
              <h1 className="font-semibold text-slate-900">Numeros de Carne:</h1>
              <p className="font-semibold text-slate-900">202308050</p>
              <p className="font-semibold text-slate-900">202408015</p>
              <p className="font-semibold text-slate-900">202408019</p>
              <p className="font-semibold text-slate-900">202308055</p>
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